#ifndef IQUOTACONTROLLER_H
#define IQUOTACONTROLLER_H

#include <StandardDefines.h>

template<typename T>
class ResponseEntity;

class QuotaResponseDto;
class QuotaSetRequestDto;

DefineStandardPointers(IQuotaController)
class IQuotaController {
    Public Virtual ~IQuotaController() = default;

    /**
     * @brief Get persisted quota rules
     */
    Public Virtual ResponseEntity<QuotaResponseDto> GetQuota() = 0;

    /**
     * @brief Update quota rules
     * @param request Body with enabled, dailyLimitLiters, steps
     * @return ResponseEntity with updated rules, or 400 if invalid
     */
    Public Virtual ResponseEntity<QuotaResponseDto> SetQuota(QuotaSetRequestDto request) = 0;
};

#include "QuotaController.h"

#endif // IQUOTACONTROLLER_H
