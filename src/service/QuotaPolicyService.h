#ifndef QUOTAPOLICYSERVICE_H
#define QUOTAPOLICYSERVICE_H

#include <StandardDefines.h>
#include "IQuotaPolicyService.h"
#include "../repository/QuotaPolicyRepository.h"
#include "../entity/QuotaPolicy.h"
#include "../util/QuotaStepsCodec.h"

/* @Service */
class QuotaPolicyService : public IQuotaPolicyService {
    Private Static constexpr Int kQuotaPolicyId = 1;
    Private Static constexpr double kDefaultDailyLimitLiters = 500.0;
    Private Static constexpr CChar* kDefaultTimezone = "UTC";

    /* @Autowired */
    Private QuotaPolicyRepositoryPtr quotaPolicyRepository;

    Public QuotaPolicyService() = default;

    Public Virtual ~QuotaPolicyService() = default;

    Public Virtual QuotaResponseDto GetQuota() override {
        optional<QuotaPolicy> entity = quotaPolicyRepository->FindById(kQuotaPolicyId);
        if (!entity.has_value()) {
            return DefaultResponse();
        }
        return ToResponse(entity.value());
    }

    Public Virtual optional<QuotaResponseDto> SetQuota(QuotaSetRequestDto request) override {
        if (!ValidateRequest(request)) {
            return optional<QuotaResponseDto>();
        }

        optional<QuotaPolicy> existing = quotaPolicyRepository->FindById(kQuotaPolicyId);

        StdVector<QuotaStepDto> steps =
            request.steps.has_value() ? QuotaStepsCodec::SortSteps(request.steps.value())
                                      : StdVector<QuotaStepDto>();

        QuotaPolicy entity;
        entity.SetId(optional<int>(kQuotaPolicyId));
        entity.SetEnabled(optional<Bool>(request.enabled.value()));
        entity.SetDailyLimitLiters(optional<double>(request.dailyLimitLiters.value()));
        entity.SetStepsJson(optional<StdString>(QuotaStepsCodec::ToJson(steps)));

        if (existing.has_value() && existing.value().GetTimezone().has_value()) {
            entity.SetTimezone(existing.value().GetTimezone());
        } else {
            entity.SetTimezone(optional<StdString>(StdString(kDefaultTimezone)));
        }

        quotaPolicyRepository->Update(entity);
        return optional<QuotaResponseDto>(ToResponse(entity));
    }

    Private Static QuotaResponseDto DefaultResponse() {
        QuotaResponseDto response;
        response.enabled = optional<Bool>(false);
        response.dailyLimitLiters = optional<double>(kDefaultDailyLimitLiters);
        response.timezone = optional<StdString>(StdString(kDefaultTimezone));
        response.steps = optional<StdVector<QuotaStepDto>>(StdVector<QuotaStepDto>());
        return response;
    }

    Private Static QuotaResponseDto ToResponse(const QuotaPolicy& entity) {
        QuotaResponseDto response;
        response.enabled = entity.GetEnabled().has_value() ? entity.GetEnabled()
                                                           : optional<Bool>(false);
        response.dailyLimitLiters =
            entity.GetDailyLimitLiters().has_value()
                ? entity.GetDailyLimitLiters()
                : optional<double>(kDefaultDailyLimitLiters);
        response.timezone =
            entity.GetTimezone().has_value() ? entity.GetTimezone()
                                             : optional<StdString>(StdString(kDefaultTimezone));

        StdVector<QuotaStepDto> steps;
        if (entity.GetStepsJson().has_value()) {
            steps = QuotaStepsCodec::FromJson(entity.GetStepsJson().value());
        }
        response.steps = optional<StdVector<QuotaStepDto>>(steps);
        return response;
    }

    Private Static Bool ValidateRequest(const QuotaSetRequestDto& request) {
        if (!request.enabled.has_value()) {
            return false;
        }
        if (!request.dailyLimitLiters.has_value() || request.dailyLimitLiters.value() <= 0.0) {
            return false;
        }

        if (!request.steps.has_value()) {
            return true;
        }

        for (const QuotaStepDto& step : request.steps.value()) {
            if (!step.atLitersUsed.has_value() || step.atLitersUsed.value() < 0.0) {
                return false;
            }
            if (!step.action.has_value()) {
                return false;
            }

            const StdString& action = step.action.value();
            if (action == "reduce_pressure") {
                if (!step.value.has_value() || step.value.value() <= 0.0) {
                    return false;
                }
            } else if (action == "turn_off") {
                continue;
            } else {
                return false;
            }
        }

        return true;
    }
};

#endif // QUOTAPOLICYSERVICE_H
