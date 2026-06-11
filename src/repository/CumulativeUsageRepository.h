#ifndef CUMULATIVE_USAGE_REPOSITORY_H
#define CUMULATIVE_USAGE_REPOSITORY_H

#include <StandardDefines.h>
#include "CpaRepository.h"
#include "../entity/CumulativeUsage.h"

/* @Repository */
DefineStandardPointers(CumulativeUsageRepository)
class CumulativeUsageRepository : public CpaRepository<CumulativeUsage, int> {
    Public Virtual ~CumulativeUsageRepository() = default;
};

#endif // CUMULATIVE_USAGE_REPOSITORY_H
