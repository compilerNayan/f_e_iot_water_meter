#ifdef ESP_PLATFORM
#ifndef SECONDMOCKFLOWSENSOR_H
#define SECONDMOCKFLOWSENSOR_H

#include <StandardDefines.h>
#include "esp_random.h"
#include "logger/ILogger.h"

/* @Component */
class SecondMockFlowSensor : public IFlowSensor {
    Private Static constexpr Int kFlowSeconds = 60;
    Private Static constexpr Int kIdleSeconds = 10;
    Private Static constexpr Int kMinMlPerSecond = 40;
    Private Static constexpr Int kMaxMlPerSecond = 60;

    Private enum class Phase { Flowing, Idle };

    Private Phase phase;
    Private Int secondsRemaining;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public SecondMockFlowSensor() : phase(Phase::Flowing), secondsRemaining(kFlowSeconds) {
        if (logger != nullptr) {
            logger->Info(Tag::Untagged,
                         "[SecondMockFlowSensor] Initialized 60s flow / 10s idle cycle");
        }
    }

    Public Virtual ~SecondMockFlowSensor() = default;

    Public Virtual Int GetMlSinceLastSecond() override {
        if (phase == Phase::Flowing) {
            Int ml = kMinMlPerSecond +
                     static_cast<Int>(esp_random() % (kMaxMlPerSecond - kMinMlPerSecond + 1));
            AdvancePhase();
            printf("Flowing: %d ml, Per Min Value: %d\n", ml, ml * 60);
            return ml;
        }

        AdvancePhase();
        printf("Idle\n");
        return 0;
    }

    Private Void AdvancePhase() {
        secondsRemaining--;
        if (secondsRemaining > 0) {
            return;
        }

        if (phase == Phase::Flowing) {
            phase = Phase::Idle;
            secondsRemaining = kIdleSeconds;
        } else {
            phase = Phase::Flowing;
            secondsRemaining = kFlowSeconds;
        }
    }
};

#endif // SECONDMOCKFLOWSENSOR_H
#endif // ESP_PLATFORM
