#ifndef IVALVESERVICE_H
#define IVALVESERVICE_H

#include <StandardDefines.h>
#include "../dto/ValveResponseDto.h"

DefineStandardPointers(IValveService)
class IValveService {
    Public Virtual ~IValveService() = default;

    /**
     * @brief Get current valve state
     * @return ValveResponseDto with target and actual percent
     */
    Public Virtual ValveResponseDto GetValveState() = 0;

    /**
     * @brief Set valve opening percent (0-100)
     * @param percent Target opening percent
     * @return ValveResponseDto after applying the change, or empty if invalid
     */
    Public Virtual optional<ValveResponseDto> SetValvePercent(Int percent) = 0;

    /**
     * @brief Re-apply persisted target to hardware actuator
     */
    Public Virtual Void RefreshValve() = 0;
};

#endif // IVALVESERVICE_H
