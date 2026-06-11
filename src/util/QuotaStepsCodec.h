#ifndef QUOTASTEPSCODEC_H
#define QUOTASTEPSCODEC_H

#include <StandardDefines.h>
#include <algorithm>
#include "../dto/QuotaStepDto.h"

class QuotaStepsCodec {
    Public Static StdVector<QuotaStepDto> SortSteps(StdVector<QuotaStepDto> steps) {
        std::sort(
            steps.begin(),
            steps.end(),
            [](const QuotaStepDto& a, const QuotaStepDto& b) {
                double aVal = a.atLitersUsed.has_value() ? a.atLitersUsed.value() : 0.0;
                double bVal = b.atLitersUsed.has_value() ? b.atLitersUsed.value() : 0.0;
                return aVal < bVal;
            });
        return steps;
    }

    Public Static StdString ToJson(const StdVector<QuotaStepDto>& steps) {
        JsonDocument doc;
        JsonArray arr = doc.to<JsonArray>();
        for (const QuotaStepDto& step : steps) {
            JsonObject obj = arr.add<JsonObject>();
            if (step.atLitersUsed.has_value()) {
                obj["atLitersUsed"] = step.atLitersUsed.value();
            }
            if (step.action.has_value()) {
                obj["action"] = step.action.value().c_str();
            }
            if (step.value.has_value()) {
                obj["value"] = step.value.value();
            }
        }
        StdString output;
        serializeJson(doc, output);
        return output;
    }

    Public Static StdVector<QuotaStepDto> FromJson(const StdString& json) {
        if (json.empty()) {
            return StdVector<QuotaStepDto>();
        }

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, json.c_str());
        if (error || !doc.is<JsonArray>()) {
            return StdVector<QuotaStepDto>();
        }

        StdVector<QuotaStepDto> steps;
        for (JsonVariant item : doc.as<JsonArray>()) {
            if (!item.is<JsonObject>()) {
                continue;
            }
            JsonObject obj = item.as<JsonObject>();
            QuotaStepDto step;
            if (!obj["atLitersUsed"].isNull()) {
                step.atLitersUsed = obj["atLitersUsed"].as<double>();
            }
            if (!obj["action"].isNull()) {
                step.action = StdString(obj["action"].as<const char*>());
            }
            if (!obj["value"].isNull()) {
                step.value = obj["value"].as<double>();
            }
            steps.push_back(step);
        }

        return SortSteps(steps);
    }
};

#endif // QUOTASTEPSCODEC_H
