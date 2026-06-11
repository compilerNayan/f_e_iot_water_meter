#ifndef QUOTARESPONSEDTO_H
#define QUOTARESPONSEDTO_H

#include <StandardDefines.h>
#include "QuotaStepDto.h"

/* @Serializable */
class QuotaResponseDto {
    Public optional<Bool> enabled;
    Public optional<double> dailyLimitLiters;
    Public optional<StdString> timezone;
    Public optional<StdVector<QuotaStepDto>> steps;

    Public QuotaResponseDto()
        : enabled(), dailyLimitLiters(), timezone(), steps() {}
};

#endif // QUOTARESPONSEDTO_H
