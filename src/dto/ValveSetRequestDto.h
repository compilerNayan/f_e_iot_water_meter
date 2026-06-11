#ifndef VALVESETREQUESTDTO_H
#define VALVESETREQUESTDTO_H

#include <StandardDefines.h>

/* @Serializable */
class ValveSetRequestDto {
    Public optional<Int> pressurePercent;

    Public ValveSetRequestDto() : pressurePercent() {}

    Public ValveSetRequestDto(CInt pressurePercent) : pressurePercent(pressurePercent) {}
};

#endif // VALVESETREQUESTDTO_H
