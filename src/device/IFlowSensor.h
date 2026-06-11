#ifndef IFLOWSENSOR_H
#define IFLOWSENSOR_H

#include <StandardDefines.h>

DefineStandardPointers(IFlowSensor)
class IFlowSensor {
    Public Virtual ~IFlowSensor() = default;

    /**
     * @brief Milliliters consumed since the previous second sample
     * @return Volume in ml for the elapsed second
     */
    Public Virtual Int GetMlSinceLastSecond() = 0;
};

#endif // IFLOWSENSOR_H
