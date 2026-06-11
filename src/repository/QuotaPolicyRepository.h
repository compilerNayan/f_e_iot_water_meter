#ifndef QUOTA_POLICY_REPOSITORY_H
#define QUOTA_POLICY_REPOSITORY_H

#include <StandardDefines.h>
#include "CpaRepository.h"
#include "../entity/QuotaPolicy.h"

/* @Repository */
DefineStandardPointers(QuotaPolicyRepository)
class QuotaPolicyRepository : public CpaRepository<QuotaPolicy, int> {
    Public Virtual ~QuotaPolicyRepository() = default;
};

#endif // QUOTA_POLICY_REPOSITORY_H
