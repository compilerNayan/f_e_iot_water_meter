#ifndef MINUTEBUCKETENTRYDTO_H
#define MINUTEBUCKETENTRYDTO_H

#include <StandardDefines.h>

/* @Serializable */
class MinuteBucketEntryDto {
    Public optional<StdString> t;
    Public optional<Int> ml;

    Public MinuteBucketEntryDto() : t(), ml() {}

    Public MinuteBucketEntryDto(CStdString t, CInt ml) : t(t), ml(ml) {}
};

#endif // MINUTEBUCKETENTRYDTO_H
