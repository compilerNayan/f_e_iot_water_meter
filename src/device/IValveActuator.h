#ifndef IVALVEACTUATOR_H
#define IVALVEACTUATOR_H

#include <StandardDefines.h>

DefineStandardPointers(IValveActuator)
class IValveActuator {
    Public Virtual ~IValveActuator() = default;

    /**
     * @brief Set valve opening percent on the given control pin
     * @param pin GPIO pin for PWM control
     * @param percent Opening percent 0-100
     */
    Public Virtual Void SetPercent(Int pin, Int percent) = 0;

    /**
     * @brief Get current valve opening percent for the given control pin
     * @param pin GPIO pin for PWM control
     * @return Opening percent 0-100
     */
    Public Virtual Int GetActualPercent(Int pin) = 0;
};

#endif // IVALVEACTUATOR_H
