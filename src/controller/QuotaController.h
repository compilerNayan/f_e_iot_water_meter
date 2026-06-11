#ifndef QUOTACONTROLLER_H
#define QUOTACONTROLLER_H

#include <StandardDefines.h>
#include "IQuotaController.h"
#include "../dto/QuotaResponseDto.h"
#include "../dto/QuotaSetRequestDto.h"
#include "ResponseEntity.h"
#include "HttpStatus.h"
#include "../service/IQuotaPolicyService.h"

/* @RestController */
/* @RequestMapping("/quota") */
class QuotaController final : public IQuotaController {
    /* @Autowired */
    Private IQuotaPolicyServicePtr quotaPolicyService;

    Public QuotaController() = default;

    Public Virtual ~QuotaController() = default;

    /* @GetMapping("") */
    Public Virtual ResponseEntity<QuotaResponseDto> GetQuota() override {
        QuotaResponseDto result = quotaPolicyService->GetQuota();
        return ResponseEntity<QuotaResponseDto>::Ok(result);
    }

    /* @PutMapping("") */
    Public Virtual ResponseEntity<QuotaResponseDto> SetQuota(
        /* @RequestBody */ QuotaSetRequestDto request) override {
        optional<QuotaResponseDto> result = quotaPolicyService->SetQuota(request);
        if (!result.has_value()) {
            return ResponseEntity<QuotaResponseDto>::BadRequest(QuotaResponseDto());
        }
        return ResponseEntity<QuotaResponseDto>::Ok(result.value());
    }
};

#endif // QUOTACONTROLLER_H
