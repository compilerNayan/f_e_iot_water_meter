#include "iot/IIotApp.h"
#include "BootTrace.h"

#include "IDeviceManager.h"
#include "service/IValveService.h"
#include "service/IFlowTelemetryService.h"

#ifdef ESP_PLATFORM
#include "esp_freertos_hooks.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#endif

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
    BootTrace_OnAppMainEnter();

#ifdef ESP_PLATFORM
    ESP_LOGI(
        "BOOT",
        "[4] ptrs valve=%p flow=%p iotApp=%p idle_hook=%p",
        static_cast<void*>(valveService.get()),
        static_cast<void*>(flowTelemetryService.get()),
        static_cast<void*>(iotApp.get()),
        reinterpret_cast<void*>(&esp_register_freertos_idle_hook_for_cpu));
    if (iotApp == nullptr) {
        ESP_LOGE("BOOT", "[4] iotApp is null, stopping");
        return;
    }
    ESP_LOGI("BOOT", "[4] calling iotApp->Start(), heap=%u", esp_get_free_heap_size());
#endif

    iotApp->Start();

#ifdef ESP_PLATFORM
    ESP_LOGI("BOOT", "[4] iotApp->Start() returned, heap=%u", esp_get_free_heap_size());
#endif

    BootTrace_OnAppMainExit();
}
