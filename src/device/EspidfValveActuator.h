#ifdef ESP_PLATFORM
#ifndef ESPIDF_VALVEACTUATOR_H
#define ESPIDF_VALVEACTUATOR_H

#include <StandardDefines.h>
#include "IValveActuator.h"
#include "driver/ledc.h"
#include "logger/ILogger.h"

#ifndef VALVE_CONTROL_PIN_INDEX
#define VALVE_CONTROL_PIN_INDEX 25
#endif

/* @Component */
class EspidfValveActuator : public IValveActuator {
    Private Static constexpr ledc_mode_t kLedcMode = LEDC_LOW_SPEED_MODE;
    Private Static constexpr ledc_timer_t kLedcTimer = LEDC_TIMER_0;
    Private Static constexpr ledc_channel_t kLedcChannel = LEDC_CHANNEL_0;
    Private Static constexpr Int kLedcFrequencyHz = 5000;
    Private Static constexpr ledc_timer_bit_t kLedcResolution = LEDC_TIMER_13_BIT;
    Private Static constexpr Int kMaxDuty = (1 << 13) - 1;

    Private StdMap<Int, Int> lastPercentByPin;
    Private Bool initialized;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public EspidfValveActuator() : lastPercentByPin(), initialized(false) {}

    Public Virtual ~EspidfValveActuator() = default;

    Public Virtual Void SetPercent(Int pin, Int percent) override {
        EnsureInitialized(pin);
        Int clamped = ClampPercent(percent);
        Int duty = (clamped * kMaxDuty) / 100;
        ledc_set_duty(kLedcMode, kLedcChannel, duty);
        ledc_update_duty(kLedcMode, kLedcChannel);
        lastPercentByPin[pin] = clamped;

        if (logger != nullptr) {
            logger->Info(
                Tag::Untagged,
                "[EspidfValveActuator] SetPercent pin=" + std::to_string(pin) +
                    " percent=" + std::to_string(clamped));
        }
    }

    Public Virtual Int GetActualPercent(Int pin) override {
        auto it = lastPercentByPin.find(pin);
        if (it != lastPercentByPin.end()) {
            return it->second;
        }
        return 0;
    }

    Private Static Int ClampPercent(Int percent) {
        if (percent < 0) {
            return 0;
        }
        if (percent > 100) {
            return 100;
        }
        return percent;
    }

    Private Void EnsureInitialized(Int pin) {
        if (initialized) {
            return;
        }

        ledc_timer_config_t timerConfig = {};
        timerConfig.speed_mode = kLedcMode;
        timerConfig.timer_num = kLedcTimer;
        timerConfig.duty_resolution = kLedcResolution;
        timerConfig.freq_hz = kLedcFrequencyHz;
        timerConfig.clk_cfg = LEDC_AUTO_CLK;
        ledc_timer_config(&timerConfig);

        ledc_channel_config_t channelConfig = {};
        channelConfig.gpio_num = pin;
        channelConfig.speed_mode = kLedcMode;
        channelConfig.channel = kLedcChannel;
        channelConfig.timer_sel = kLedcTimer;
        channelConfig.duty = 0;
        channelConfig.hpoint = 0;
        ledc_channel_config(&channelConfig);

        initialized = true;
        lastPercentByPin[pin] = 0;

        if (logger != nullptr) {
            logger->Info(
                Tag::Untagged,
                "[EspidfValveActuator] Initialized PWM on pin=" + std::to_string(pin));
        }
    }
};

#endif // ESPIDF_VALVEACTUATOR_H
#endif // ESP_PLATFORM
