#include "iot/IIotApp.h"

#include "IDeviceManager.h"
#include "service/IValveService.h"

/*--@Autowired--*/
IValveServicePtr valveService = Implementation<IValveService>::type::GetInstance();

Void DeviceManager::Setup() {
}

Void DeviceManager::Loop() {
    valveService->RefreshValve();
}

/*--@Autowired--*/
IIotAppPtr iotApp = Implementation<IIotApp>::type::GetInstance();

extern "C" void app_main(void) {
    iotApp->Start();
}
