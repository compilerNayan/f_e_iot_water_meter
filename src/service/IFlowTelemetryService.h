#ifndef IFLOWTELEMETRYSERVICE_H
#define IFLOWTELEMETRYSERVICE_H

#include <StandardDefines.h>

DefineStandardPointers(IFlowTelemetryService)
class IFlowTelemetryService {
    Public Virtual ~IFlowTelemetryService() = default;

    /**
     * @brief Process one telemetry tick (call from DeviceManager loop)
     */
    Public Virtual Void Tick() = 0;
};

#endif // IFLOWTELEMETRYSERVICE_H
