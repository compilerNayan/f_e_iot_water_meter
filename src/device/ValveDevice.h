#ifndef VALVEDEVICE_H
#define VALVEDEVICE_H

#include <StandardDefines.h>
#include "IValveDevice.h"
#include "IValveActuator.h"
#include "logger/ILogger.h"
#include "../repository/ValveRepository.h"
#include "../entity/Valve.h"
#include "../dto/ValveResponseDto.h"

#ifndef VALVE_CONTROL_PIN_INDEX
#define VALVE_CONTROL_PIN_INDEX 25
#endif

/* @Component */
class ValveDevice : public IValveDevice {
    Private Static constexpr Int kValveId = 1;
    Private Static constexpr Int kDefaultTargetPercent = 100;

    Private Int controlPin;
    Private Int targetPressurePercent;

    /* @Autowired */
    Private IValveActuatorPtr valveActuator;

    /* @Autowired */
    Private ILoggerPtr logger;

    /* @Autowired */
    Private ValveRepositoryPtr valveRepository;

    Public ValveDevice() : controlPin(VALVE_CONTROL_PIN_INDEX), targetPressurePercent(kDefaultTargetPercent) {
        optional<Valve> valveEntity = valveRepository->FindById(kValveId);
        if (valveEntity.has_value() && valveEntity.value().GetTargetPressurePercent().has_value()) {
            targetPressurePercent = valveEntity.value().GetTargetPressurePercent().value();
        }

        valveActuator->SetPercent(controlPin, targetPressurePercent);
    }

    Public Virtual ~ValveDevice() = default;

    Public Virtual Void SetTargetPercent(Int percent) override {
        Int clamped = ClampPercent(percent);
        targetPressurePercent = clamped;

        Valve valveEntity;
        valveEntity.SetId(optional<int>(kValveId));
        valveEntity.SetTargetPressurePercent(optional<int>(targetPressurePercent));
        valveRepository->Update(valveEntity);

        valveActuator->SetPercent(controlPin, targetPressurePercent);

        if (logger != nullptr) {
            logger->Info(
                Tag::Untagged,
                "Set valve target to " + std::to_string(targetPressurePercent) + "%");
        }
    }

    Public Virtual ValveResponseDto GetValveDetails() override {
        Int actual = valveActuator->GetActualPercent(controlPin);
        return ValveResponseDto(targetPressurePercent, actual);
    }

    Public Virtual Void Refresh() override {
        Int actual = valveActuator->GetActualPercent(controlPin);
        if (actual != targetPressurePercent) {
            valveActuator->SetPercent(controlPin, targetPressurePercent);
            if (logger != nullptr) {
                logger->Info(
                    Tag::Untagged,
                    "Refreshed valve actuator to " + std::to_string(targetPressurePercent) + "%");
            }
        }
    }

    Private Static Int ClampPercent(Int percent) {
        if (percent < 0) {
            return 0;
        }
        if (percent > 100) {
            return 100;
        }
        return percent;
    }
};

#endif // VALVEDEVICE_H
