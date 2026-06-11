#ifndef VALVECONTROLLER_H
#define VALVECONTROLLER_H

#include <StandardDefines.h>
#include "IValveController.h"
#include "../dto/ValveResponseDto.h"
#include "../dto/ValveSetRequestDto.h"
#include "ResponseEntity.h"
#include "HttpStatus.h"
#include "../service/IValveService.h"

/* @RestController */
/* @RequestMapping("/valve") */
class ValveController final : public IValveController {
    /* @Autowired */
    Private IValveServicePtr valveService;

    Public ValveController() = default;

    Public Virtual ~ValveController() = default;

    /* @GetMapping("") */
    Public Virtual ResponseEntity<ValveResponseDto> GetValveState() override {
        ValveResponseDto result = valveService->GetValveState();
        return ResponseEntity<ValveResponseDto>::Ok(result);
    }

    /* @PutMapping("") */
    Public Virtual ResponseEntity<ValveResponseDto> SetValveState(
        /* @RequestBody */ ValveSetRequestDto request) override {
        if (!request.pressurePercent.has_value()) {
            return ResponseEntity<ValveResponseDto>::BadRequest(ValveResponseDto());
        }

        optional<ValveResponseDto> result =
            valveService->SetValvePercent(request.pressurePercent.value());
        if (!result.has_value()) {
            return ResponseEntity<ValveResponseDto>::BadRequest(ValveResponseDto());
        }
        return ResponseEntity<ValveResponseDto>::Ok(result.value());
    }
};

#endif // VALVECONTROLLER_H
