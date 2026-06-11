#ifndef VALVE_H
#define VALVE_H

#include <StandardDefines.h>

/* @Entity */
class Valve {

    /* @Id */
    /* @NotNull */
    Public optional<int> id;

    Public optional<int> targetPressurePercent;

    Public Valve() : id(), targetPressurePercent() {}

    Public Valve(optional<int> id, optional<int> targetPressurePercent)
        : id(id), targetPressurePercent(targetPressurePercent) {}

    Public optional<int> GetId() const {
        return id;
    }

    Public Void SetId(optional<int> id) {
        this->id = id;
    }

    Public optional<int> GetTargetPressurePercent() const {
        return targetPressurePercent;
    }

    Public Void SetTargetPressurePercent(optional<int> targetPressurePercent) {
        this->targetPressurePercent = targetPressurePercent;
    }
};

#endif // VALVE_H
