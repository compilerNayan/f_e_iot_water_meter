#include "iot/IIotApp.h"

#include "IDeviceManager.h"


Void DeviceManager::Setup() {
}

Void DeviceManager::Loop() {

}

/*--@Autowired--*/
IIotAppPtr iotApp = Implementation<IIotApp>::type::GetInstance();

extern "C" void app_main(void) {
    iotApp->Start();
}