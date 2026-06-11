#ifdef AAMIIE
#ifdef ESP_PLATFORM
#ifndef YFS201FLOWSENSOR_H
#define YFS201FLOWSENSOR_H

#include <StandardDefines.h>
#include "IFlowSensor.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "logger/ILogger.h"

#ifndef FLOW_SENSOR_PULSE_PIN
#define FLOW_SENSOR_PULSE_PIN 18
#endif

#ifndef FLOW_SENSOR_PULSES_PER_LITER
#define FLOW_SENSOR_PULSES_PER_LITER 450
#endif

static DRAM_ATTR volatile UInt32 gYfS201PulseCount = 0;

/* @Component */
class YfS201FlowSensor : public IFlowSensor {
    Private Static constexpr Int kPulsesPerLiter = FLOW_SENSOR_PULSES_PER_LITER;
    Private Static constexpr Int kPulsePin = FLOW_SENSOR_PULSE_PIN;

    Private Int lastPulseCount;
    Private Bool initialized;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public YfS201FlowSensor() : lastPulseCount(0), initialized(false) {
        EnsureInitialized();
    }

    Public Virtual ~YfS201FlowSensor() = default;

    Public Virtual Int GetMlSinceLastSecond() override {
        EnsureInitialized();
        UInt32 currentCount = gYfS201PulseCount;
        UInt32 delta = currentCount - static_cast<UInt32>(lastPulseCount);
        lastPulseCount = static_cast<Int>(currentCount);
        if (delta == 0) {
            return 0;
        }
        return static_cast<Int>((static_cast<UInt64>(delta) * 1000ULL) / static_cast<UInt64>(kPulsesPerLiter));
    }

    Private Static Void OnPulseEdge(Void* arg) {
        (void)arg;
        gYfS201PulseCount = gYfS201PulseCount + 1;
    }

    Private Void EnsureInitialized() {
        if (initialized) {
            return;
        }

        gYfS201PulseCount = 0;
        lastPulseCount = 0;

        gpio_config_t ioConfig = {};
        ioConfig.intr_type = GPIO_INTR_POSEDGE;
        ioConfig.mode = GPIO_MODE_INPUT;
        ioConfig.pin_bit_mask = (1ULL << kPulsePin);
        ioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
        ioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&ioConfig);

        esp_err_t isrResult = gpio_install_isr_service(0);
        if (isrResult != ESP_OK && isrResult != ESP_ERR_INVALID_STATE) {
            return;
        }
        gpio_isr_handler_add(static_cast<gpio_num_t>(kPulsePin), OnPulseEdge, nullptr);

        initialized = true;

        if (logger != nullptr) {
            logger->Info(
                Tag::Untagged,
                "[YfS201FlowSensor] Initialized GPIO ISR on pin=" + std::to_string(kPulsePin) +
                    " pulsesPerLiter=" + std::to_string(kPulsesPerLiter));
        }
    }
};

#endif // YFS201FLOWSENSOR_H
#endif // ESP_PLATFORM
#endif // AAMIIE