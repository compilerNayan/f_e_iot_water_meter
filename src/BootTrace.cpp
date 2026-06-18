#ifdef ESP_PLATFORM

#include <cstdio>

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_private/startup_internal.h"
#include "esp_system.h"

static const char* kTag = "BOOT";

ESP_SYSTEM_INIT_FN(boot_trace_pre_coredump, SECONDARY, BIT(0), 129)
{
    ESP_EARLY_LOGI(kTag, "[1] before coredump init");
    return ESP_OK;
}

ESP_SYSTEM_INIT_FN(boot_trace_post_coredump, SECONDARY, BIT(0), 131)
{
    ESP_EARLY_LOGI(kTag, "[2] after coredump init");
    return ESP_OK;
}

__attribute__((constructor(65535))) static void BootTrace_AfterStaticInit()
{
    printf("[BOOT] [DI] static init done, heap=%lu\n",
        static_cast<unsigned long>(esp_get_free_heap_size()));
}

extern "C" void BootTrace_OnAppMainEnter()
{
    ESP_LOGI(kTag, "[3] app_main enter, heap=%u", esp_get_free_heap_size());
}

extern "C" void BootTrace_OnAppMainExit()
{
    ESP_LOGI(kTag, "[5] app_main exit, heap=%u", esp_get_free_heap_size());
}

#endif // ESP_PLATFORM
