#ifndef IQUOTAPOLICYSERVICE_H
#define IQUOTAPOLICYSERVICE_H

#include <StandardDefines.h>
#include "../dto/QuotaResponseDto.h"
#include "../dto/QuotaSetRequestDto.h"

DefineStandardPointers(IQuotaPolicyService)
class IQuotaPolicyService {
    Public Virtual ~IQuotaPolicyService() = default;

    /**
     * @brief Get persisted quota rules, or defaults if none stored
     */
    Public Virtual QuotaResponseDto GetQuota() = 0;

    /**
     * @brief Validate and persist quota rules
     * @return Updated rules, or empty if validation failed
     */
    Public Virtual optional<QuotaResponseDto> SetQuota(QuotaSetRequestDto request) = 0;
};

#endif // IQUOTAPOLICYSERVICE_H
