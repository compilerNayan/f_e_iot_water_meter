#ifndef THIRTYMINUTEBUCKETDTO_H
#define THIRTYMINUTEBUCKETDTO_H

#include <StandardDefines.h>
#include "MinuteBucketEntryDto.h"

/* @Serializable */
class ThirtyMinuteBucketDto {
    Public optional<StdString> tenantId;
    Public optional<StdString> deviceId;
    Public optional<StdString> periodStart;
    Public StdVector<MinuteBucketEntryDto> minutes;
    Public optional<double> cumulativeLiters;
    Public optional<Int> valveTargetPercent;

    Public ThirtyMinuteBucketDto()
        : tenantId(),
          deviceId(),
          periodStart(),
          minutes(),
          cumulativeLiters(),
          valveTargetPercent() {}
};

#endif // THIRTYMINUTEBUCKETDTO_H
