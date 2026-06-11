#include "iot/IIotApp.h"

#include "IDeviceManager.h"
#include "service/IValveService.h"
#include "service/IFlowTelemetryService.h"

/* @Autowired */
IValveServicePtr valveService;

/* @Autowired */
IFlowTelemetryServicePtr flowTelemetryService;

Void DeviceManager::Setup() {
}

Void DeviceManager::Loop() {
    valveService->RefreshValve();
    flowTelemetryService->Tick();
}

/* @Autowired */
IIotAppPtr iotApp;

extern "C" void app_main(void) {
    iotApp->Start();
}
