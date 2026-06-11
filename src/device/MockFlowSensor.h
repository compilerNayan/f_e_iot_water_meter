#ifdef ESP_PLATFORM
#ifndef MOCKFLOWSENSOR_H
#define MOCKFLOWSENSOR_H

#include <StandardDefines.h>
#include "esp_random.h"
#include "logger/ILogger.h"

/* @Component */
class MockFlowSensor : public IFlowSensor {
    Private Int currentMlPerSecond;
    Private Int burstRemaining;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public MockFlowSensor() : currentMlPerSecond(0), burstRemaining(0) {
        if (logger != nullptr) {
            logger->Info(Tag::Untagged, "[MockFlowSensor] Initialized synthetic household flow");
        }
    }

    Public Virtual ~MockFlowSensor() = default;

    Public Virtual Int GetMlSinceLastSecond() override {
        if (burstRemaining > 0) {
            burstRemaining--;
            return currentMlPerSecond;
        }

        currentMlPerSecond = SampleFlowPool();

        if (currentMlPerSecond > 0) {
            burstRemaining = 30 + static_cast<Int>(esp_random() % 271);
        } else {
            burstRemaining = 60 + static_cast<Int>(esp_random() % 541);
        }

        return currentMlPerSecond;
    }

    Private Static Int SampleFlowPool() {
        UInt32 roll = esp_random() % 100;

        if (roll < 70) {
            return 0;
        }
        if (roll < 80) {
            return 50 + static_cast<Int>(esp_random() % 51);
        }
        if (roll < 90) {
            return 100 + static_cast<Int>(esp_random() % 51);
        }
        return 150 + static_cast<Int>(esp_random() % 51);
    }
};

#endif // MOCKFLOWSENSOR_H
#endif // ESP_PLATFORM
