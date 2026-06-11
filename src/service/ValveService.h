#ifndef VALVESERVICE_H
#define VALVESERVICE_H

#include <StandardDefines.h>
#include "IValveService.h"
#include "../device/IValveDevice.h"
#include "../dto/ValveResponseDto.h"

/* @Service */
class ValveService : public IValveService {
    /* @Autowired */
    Private IValveDevicePtr valveDevice;

    Public ValveService() = default;

    Public Virtual ~ValveService() = default;

    Public Virtual ValveResponseDto GetValveState() override {
        return valveDevice->GetValveDetails();
    }

    Public Virtual optional<ValveResponseDto> SetValvePercent(Int percent) override {
        if (percent < 0 || percent > 100) {
            return optional<ValveResponseDto>();
        }
        valveDevice->SetTargetPercent(percent);
        return optional<ValveResponseDto>(valveDevice->GetValveDetails());
    }

    Public Virtual Void RefreshValve() override {
        valveDevice->Refresh();
    }
};

#endif // VALVESERVICE_H
