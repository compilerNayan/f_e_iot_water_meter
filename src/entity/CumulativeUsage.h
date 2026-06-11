#ifndef CUMULATIVEUSAGE_H
#define CUMULATIVEUSAGE_H

#include <StandardDefines.h>

/* @Entity */
class CumulativeUsage {

    /* @Id */
    /* @NotNull */
    Public optional<int> id;

    Public optional<double> cumulativeLiters;

    Public optional<StdString> updatedAt;

    Public CumulativeUsage() : id(), cumulativeLiters(), updatedAt() {}

    Public CumulativeUsage(optional<int> id, optional<double> cumulativeLiters, optional<StdString> updatedAt)
        : id(id), cumulativeLiters(cumulativeLiters), updatedAt(updatedAt) {}

    Public optional<int> GetId() const {
        return id;
    }

    Public Void SetId(optional<int> id) {
        this->id = id;
    }

    Public optional<double> GetCumulativeLiters() const {
        return cumulativeLiters;
    }

    Public Void SetCumulativeLiters(optional<double> cumulativeLiters) {
        this->cumulativeLiters = cumulativeLiters;
    }

    Public optional<StdString> GetUpdatedAt() const {
        return updatedAt;
    }

    Public Void SetUpdatedAt(optional<StdString> updatedAt) {
        this->updatedAt = updatedAt;
    }
};

#endif // CUMULATIVEUSAGE_H
