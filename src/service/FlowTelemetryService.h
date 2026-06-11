#ifndef FLOWTELEMETRYSERVICE_H
#define FLOWTELEMETRYSERVICE_H

#include <StandardDefines.h>
#include <ctime>
#include <sstream>
#include "IFlowTelemetryService.h"
#include "../device/IFlowSensor.h"
#include "../device/IValveDevice.h"
#include "../dto/ValveResponseDto.h"
#include "../dto/MinuteBucketEntryDto.h"
#include "../repository/CumulativeUsageRepository.h"
#include "repository/CumulativeUsageRepositoryImpl.h"
#include "../entity/CumulativeUsage.h"
#include "communication/IServerProvider.h"
#include "service/IConnectionDetailsProvider.h"
#include "esp_timer.h"

/* @Service */
class FlowTelemetryService : public IFlowTelemetryService {
    Private Static constexpr Int kUsageId = 1;
    Private Static constexpr Int kWindowSize = 30;
    Private Static constexpr Int64 kSecondUs = 1000000;

    /* @Autowired */
    Private IFlowSensorPtr flowSensor;

    /* @Autowired */
    Private IServerProviderPtr serverProvider;

    /* @Autowired */
    Private CumulativeUsageRepositoryPtr cumulativeUsageRepository;

    /* @Autowired */
    Private IValveDevicePtr valveDevice;

    /* @Autowired */
    Private IConnectionDetailsProviderPtr connectionDetailsProvider;

    Private Int64 lastSecondTickUs;
    Private Int lastMinuteKey;
    Private struct tm lastMinuteUtc;
    Private Bool minuteInitialized;
    Private Int minuteMl;
    Private double cumulativeLiters;
    Private StdVector<MinuteBucketEntryDto> windowMinutes;

    Public FlowTelemetryService()
        : lastSecondTickUs(0),
          lastMinuteKey(-1),
          lastMinuteUtc{},
          minuteInitialized(false),
          minuteMl(0),
          cumulativeLiters(0.0),
          windowMinutes() {
        LoadCumulativeFromStore();
    }

    Public Virtual ~FlowTelemetryService() = default;

    Public Virtual Void Tick() override {
        Int64 nowUs = esp_timer_get_time();
        if (lastSecondTickUs != 0 && (nowUs - lastSecondTickUs) < kSecondUs) {
            return;
        }
        lastSecondTickUs = nowUs;

        if (!IsTimeValid()) {
            return;
        }

        HandleMinuteBoundaryIfNeeded();

        Int ml = flowSensor->GetMlSinceLastSecond();
        minuteMl += ml;
        cumulativeLiters += static_cast<double>(ml) / 1000.0;

        if (ml > 0) {
            PublishSecondPulse(ml);
        }
    }

    Private Bool IsTimeValid() {
        return time(nullptr) > 1609459200;
    }

    Private Void HandleMinuteBoundaryIfNeeded() {
        time_t now = time(nullptr);
        struct tm utc;
        gmtime_r(&now, &utc);
        Int minuteKey = MinuteKey(utc);

        if (!minuteInitialized) {
            lastMinuteKey = minuteKey;
            lastMinuteUtc = utc;
            minuteInitialized = true;
            return;
        }

        if (minuteKey == lastMinuteKey) {
            return;
        }

        AppendCompletedMinute(lastMinuteUtc, minuteMl);
        PersistCumulative(utc);

        if (lastMinuteUtc.tm_min % 30 == 0) {
            PublishThirtyMinuteBucket();
            windowMinutes.clear();
        }

        minuteMl = 0;
        lastMinuteKey = minuteKey;
        lastMinuteUtc = utc;
    }

    Private Void AppendCompletedMinute(const struct tm& minuteUtc, Int ml) {
        windowMinutes.push_back(MinuteBucketEntryDto(FormatUtcMinute(minuteUtc), ml));
        while (static_cast<Int>(windowMinutes.size()) > kWindowSize) {
            windowMinutes.erase(windowMinutes.begin());
        }
    }

    Private Void PublishSecondPulse(Int ml) {
        ICloudServerPtr cloudServer = serverProvider->GetCloudServerPtr();
        if (cloudServer == nullptr) {
            return;
        }
        time_t now = time(nullptr);
        struct tm utc;
        gmtime_r(&now, &utc);
        StdString ts = FormatUtcMinute(utc);
        StdString payload = "{\"ts\":\"" + ts + "\",\"ml\":" + std::to_string(ml) + "}";
        cloudServer->PublishSecondPulse(payload);
    }

    Private Void PublishThirtyMinuteBucket() {
        if (windowMinutes.empty()) {
            return;
        }
        ICloudServerPtr cloudServer = serverProvider->GetCloudServerPtr();
        if (cloudServer == nullptr) {
            return;
        }

        StdString minutesJson = "[";
        for (size_t i = 0; i < windowMinutes.size(); ++i) {
            if (i > 0) {
                minutesJson += ",";
            }
            StdString minuteTime = windowMinutes[i].t.has_value() ? windowMinutes[i].t.value() : "";
            Int minuteMlValue = windowMinutes[i].ml.has_value() ? windowMinutes[i].ml.value() : 0;
            minutesJson += "{\"t\":\"" + minuteTime + "\",\"ml\":" + std::to_string(minuteMlValue) + "}";
        }
        minutesJson += "]";

        Int valveTarget = 100;
        ValveResponseDto valveState = valveDevice->GetValveDetails();
        if (valveState.targetPressurePercent.has_value()) {
            valveTarget = valveState.targetPressurePercent.value();
        }

        StdString payload = "{\"tenantId\":\"" + connectionDetailsProvider->GetTenantId()
            + "\",\"deviceId\":\"" + connectionDetailsProvider->GetThingName()
            + "\",\"periodStart\":\"" + (windowMinutes.front().t.has_value() ? windowMinutes.front().t.value() : "")
            + "\",\"minutes\":" + minutesJson
            + ",\"cumulativeLiters\":" + std::to_string(cumulativeLiters)
            + ",\"valveTargetPercent\":" + std::to_string(valveTarget) + "}";

        cloudServer->PublishThirtyMinuteBucket(payload);
    }

    Private Void LoadCumulativeFromStore() {
        optional<CumulativeUsage> entity = cumulativeUsageRepository->FindById(kUsageId);
        if (entity.has_value() && entity.value().GetCumulativeLiters().has_value()) {
            cumulativeLiters = entity.value().GetCumulativeLiters().value();
        }
    }

    Private Void PersistCumulative(const struct tm& utc) {
        CumulativeUsage entity;
        entity.SetId(optional<int>(kUsageId));
        entity.SetCumulativeLiters(optional<double>(cumulativeLiters));
        entity.SetUpdatedAt(optional<StdString>(FormatUtcMinute(utc)));
        cumulativeUsageRepository->Update(entity);
    }

    Private Static Int MinuteKey(const struct tm& utc) {
        return ((utc.tm_year + 1900) * 1000000) + ((utc.tm_mon + 1) * 10000)
             + (utc.tm_mday * 100) + utc.tm_hour * 60 + utc.tm_min;
    }

    Private Static StdString FormatUtcMinute(const struct tm& utc) {
        char buffer[32];
        struct tm copy = utc;
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &copy);
        return StdString(buffer);
    }
};

#endif // FLOWTELEMETRYSERVICE_H
