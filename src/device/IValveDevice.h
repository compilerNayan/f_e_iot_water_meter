#ifndef IVALVEDEVICE_H
#define IVALVEDEVICE_H

#include <StandardDefines.h>
#include "../dto/ValveResponseDto.h"

DefineStandardPointers(IValveDevice)
class IValveDevice {
    Public Virtual ~IValveDevice() = default;

    /**
     * @brief Set target valve opening percent (0-100)
     * @param percent Target opening percent
     */
    Public Virtual Void SetTargetPercent(Int percent) = 0;

    /**
     * @brief Get current valve state details
     * @return ValveResponseDto with target and actual percent
     */
    Public Virtual ValveResponseDto GetValveDetails() = 0;

    /**
     * @brief Re-apply persisted target to hardware actuator
     */
    Public Virtual Void Refresh() = 0;
};

#endif // IVALVEDEVICE_H
