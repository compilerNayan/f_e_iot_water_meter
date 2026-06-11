#ifndef IVALVECONTROLLER_H
#define IVALVECONTROLLER_H

#include <StandardDefines.h>

template<typename T>
class ResponseEntity;

class ValveResponseDto;
class ValveSetRequestDto;

DefineStandardPointers(IValveController)
class IValveController {
    Public Virtual ~IValveController() = default;

    /**
     * @brief Get current valve state
     * @return ResponseEntity with valve target and actual percent
     */
    Public Virtual ResponseEntity<ValveResponseDto> GetValveState() = 0;

    /**
     * @brief Set valve opening percent
     * @param request Body with pressurePercent 0-100
     * @return ResponseEntity with updated valve state, or 400 if invalid
     */
    Public Virtual ResponseEntity<ValveResponseDto> SetValveState(ValveSetRequestDto request) = 0;
};

#include "ValveController.h"
#include "/Users/sexydevil/src/automation_src/fresh/f_e_iot_water_meter/src/controller/ValveController.h"
#endif // IVALVECONTROLLER_H
