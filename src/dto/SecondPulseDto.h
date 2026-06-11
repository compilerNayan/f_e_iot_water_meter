#ifndef SECONDPULSEDTO_H
#define SECONDPULSEDTO_H

#include <StandardDefines.h>

/* @Serializable */
class SecondPulseDto {
    Public optional<StdString> ts;
    Public optional<Int> ml;

    Public SecondPulseDto() : ts(), ml() {}

    Public SecondPulseDto(CStdString ts, CInt ml) : ts(ts), ml(ml) {}
};

#endif // SECONDPULSEDTO_H
