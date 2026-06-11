#ifndef QUOTASTEPDTO_H
#define QUOTASTEPDTO_H

#include <StandardDefines.h>

/* @Serializable */
class QuotaStepDto {
    Public optional<double> atLitersUsed;
    Public optional<StdString> action;
    Public optional<double> value;

    Public QuotaStepDto() : atLitersUsed(), action(), value() {}

    Public QuotaStepDto(CDouble atLitersUsed, CStdString action, optional<double> value)
        : atLitersUsed(atLitersUsed), action(action), value(value) {}
};

#endif // QUOTASTEPDTO_H
