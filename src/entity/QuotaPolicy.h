#ifndef QUOTAPOLICY_H
#define QUOTAPOLICY_H

#include <StandardDefines.h>

/* @Entity */
class QuotaPolicy {

    /* @Id */
    /* @NotNull */
    Public optional<int> id;

    Public optional<Bool> enabled;

    Public optional<double> dailyLimitLiters;

    Public optional<StdString> stepsJson;

    Public optional<StdString> timezone;

    Public QuotaPolicy()
        : id(), enabled(), dailyLimitLiters(), stepsJson(), timezone() {}

    Public QuotaPolicy(
        optional<int> id,
        optional<Bool> enabled,
        optional<double> dailyLimitLiters,
        optional<StdString> stepsJson,
        optional<StdString> timezone)
        : id(id),
          enabled(enabled),
          dailyLimitLiters(dailyLimitLiters),
          stepsJson(stepsJson),
          timezone(timezone) {}

    Public optional<int> GetId() const {
        return id;
    }

    Public Void SetId(optional<int> id) {
        this->id = id;
    }

    Public optional<Bool> GetEnabled() const {
        return enabled;
    }

    Public Void SetEnabled(optional<Bool> enabled) {
        this->enabled = enabled;
    }

    Public optional<double> GetDailyLimitLiters() const {
        return dailyLimitLiters;
    }

    Public Void SetDailyLimitLiters(optional<double> dailyLimitLiters) {
        this->dailyLimitLiters = dailyLimitLiters;
    }

    Public optional<StdString> GetStepsJson() const {
        return stepsJson;
    }

    Public Void SetStepsJson(optional<StdString> stepsJson) {
        this->stepsJson = stepsJson;
    }

    Public optional<StdString> GetTimezone() const {
        return timezone;
    }

    Public Void SetTimezone(optional<StdString> timezone) {
        this->timezone = timezone;
    }
};

#endif // QUOTAPOLICY_H
