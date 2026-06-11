#ifndef QUOTASETREQUESTDTO_H
#define QUOTASETREQUESTDTO_H

#include <StandardDefines.h>
#include "QuotaStepDto.h"

/* @Serializable */
class QuotaSetRequestDto {
    Public optional<Bool> enabled;
    Public optional<double> dailyLimitLiters;
    Public optional<StdVector<QuotaStepDto>> steps;

    Public QuotaSetRequestDto() : enabled(), dailyLimitLiters(), steps() {}
};

#endif // QUOTASETREQUESTDTO_H
