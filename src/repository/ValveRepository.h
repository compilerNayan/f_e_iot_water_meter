#ifndef VALVE_REPOSITORY_H
#define VALVE_REPOSITORY_H

#include <StandardDefines.h>
#include "CpaRepository.h"
#include "../entity/Valve.h"

/* @Repository */
DefineStandardPointers(ValveRepository)
class ValveRepository : public CpaRepository<Valve, int> {
    Public Virtual ~ValveRepository() = default;
};

#endif // VALVE_REPOSITORY_H
