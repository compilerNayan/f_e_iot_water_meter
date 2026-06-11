#ifndef VALVERESPONSEDTO_H
#define VALVERESPONSEDTO_H

#include <StandardDefines.h>

/* @Serializable */
class ValveResponseDto {
    Public optional<Int> targetPressurePercent;
    Public optional<Int> actualPressurePercent;

    Public ValveResponseDto() : targetPressurePercent(), actualPressurePercent() {}

    Public ValveResponseDto(CInt targetPressurePercent, CInt actualPressurePercent)
        : targetPressurePercent(targetPressurePercent),
          actualPressurePercent(actualPressurePercent) {}
};

#endif // VALVERESPONSEDTO_H
