#include "ac_intent_ingress.h"

#include "ac_preset_registry.h"
#include "ac_state_memory.h"
#include "ac_temperature_registry.h"

#include <stdio.h>
#include <string.h>

namespace {

static bool parse_fixed_temp_alias(const char* command, AcIntent* out_intent) {
    if (command == nullptr || out_intent == nullptr) {
        return false;
    }

    if (const AcTemperatureAssetRecord* asset = ac_temperature_registry_find_by_command(command); asset != nullptr) {
        if (asset->status != AC_TEMPERATURE_ASSET_VERIFIED || asset->preset_id == AC_PRESET_NONE) {
            return false;
        }
        *out_intent = {true, true, true, asset->temp_c, asset->mode, AC_FAN_AUTO, false, false, asset->preset_id};
        return true;
    }

    return false;
}

}  // namespace

bool ac_intent_ingress_parse_command(const AcProfile* profile, const char* command, AcIntent* out_intent) {
    if (out_intent == nullptr) {
        return false;
    }

    *out_intent = {
        .power_set = false,
        .power_on = false,
        .temp_set = false,
        .temp_c = 0,
        .mode = AC_MODE_UNKNOWN,
        .fan = AC_FAN_UNKNOWN,
        .sleep_set = false,
        .sleep_on = false,
        .preset_hint = AC_PRESET_NONE,
    };

    if (command == nullptr || command[0] == '\0') {
        return false;
    }

    const AcStateSnapshot state = ac_state_memory_get();
    const AcMode remembered_mode = state.last_intent.mode == AC_MODE_HEAT ? AC_MODE_HEAT : AC_MODE_COOL;

    if (strcmp(command, "AC_FAN_1") == 0 || strcmp(command, "AC_FAN_2") == 0 ||
        strcmp(command, "AC_FAN_3") == 0 || strcmp(command, "AC_FAN_5") == 0) {
        const AcFan target_fan =
            strcmp(command, "AC_FAN_1") == 0 ? AC_FAN_1 :
            strcmp(command, "AC_FAN_2") == 0 ? AC_FAN_2 :
            strcmp(command, "AC_FAN_3") == 0 ? AC_FAN_3 :
            AC_FAN_5;
        const AcPresetId target_preset =
            remembered_mode == AC_MODE_HEAT
                ? (target_fan == AC_FAN_1 ? AC_PRESET_HEAT_26_FAN1 :
                   target_fan == AC_FAN_2 ? AC_PRESET_HEAT_26_FAN2 :
                   target_fan == AC_FAN_3 ? AC_PRESET_HEAT_26_FAN3 :
                                            AC_PRESET_HEAT_26_FAN5)
                : (target_fan == AC_FAN_1 ? AC_PRESET_COOL_26_FAN1 :
                   target_fan == AC_FAN_2 ? AC_PRESET_COOL_26_FAN2 :
                   target_fan == AC_FAN_3 ? AC_PRESET_COOL_26_FAN3 :
                                            AC_PRESET_COOL_26_FAN5);
        *out_intent = {
            .power_set = true,
            .power_on = true,
            .temp_set = true,
            .temp_c = 26,
            .mode = remembered_mode,
            .fan = target_fan,
            .sleep_set = false,
            .sleep_on = false,
            .preset_hint = target_preset,
        };
        return true;
    }

    if (strcmp(command, "AC_SLEEP_ON_HEAT") == 0 || strcmp(command, "AC_SLEEP_ON_COOL") == 0) {
        const AcPresetId target_preset =
            remembered_mode == AC_MODE_HEAT ? AC_PRESET_HEAT_SLEEP_ON : AC_PRESET_COOL_SLEEP_ON;
        *out_intent = {
            .power_set = true,
            .power_on = true,
            .temp_set = false,
            .temp_c = 0,
            .mode = remembered_mode,
            .fan = AC_FAN_UNKNOWN,
            .sleep_set = true,
            .sleep_on = true,
            .preset_hint = target_preset,
        };
        return true;
    }

    if (strcmp(command, "AC_TEMP_UP") == 0 || strcmp(command, "AC_TEMP_DOWN") == 0) {
        const int delta = strcmp(command, "AC_TEMP_UP") == 0 ? 1 : -1;
        int next_temp = static_cast<int>(state.last_intent.temp_c);
        const bool has_reference_temp =
            state.last_intent.temp_set &&
            state.last_intent.temp_c >= (profile ? profile->temp_min_c : 16) &&
            state.last_intent.temp_c <= (profile ? profile->temp_max_c : 30);

        if (!has_reference_temp) {
            next_temp = 25;
        }
        next_temp += delta;
        const int temp_min = profile ? profile->temp_min_c : 16;
        const int temp_max = profile ? profile->temp_max_c : 30;
        if (next_temp < temp_min) next_temp = temp_min;
        if (next_temp > temp_max) next_temp = temp_max;
        const AcMode target_mode =
            state.last_intent.mode == AC_MODE_HEAT ? AC_MODE_HEAT : AC_MODE_COOL;
        const AcTemperatureAssetRecord* asset =
            ac_temperature_registry_find_verified_by_mode_temp(target_mode, static_cast<uint8_t>(next_temp));
        if (asset == nullptr || asset->preset_id == AC_PRESET_NONE) {
            if (!has_reference_temp && target_mode == AC_MODE_COOL) {
                asset = ac_temperature_registry_find_verified_by_mode_temp(AC_MODE_COOL, 25);
                next_temp = 25;
            }
        }
        if (asset == nullptr || asset->preset_id == AC_PRESET_NONE) {
            return false;
        }

        *out_intent = state.last_intent;
        out_intent->power_set = true;
        out_intent->power_on = true;
        out_intent->temp_set = true;
        out_intent->temp_c = static_cast<uint8_t>(next_temp);
        out_intent->mode = target_mode;
        out_intent->preset_hint = asset->preset_id;
        return true;
    }

    if (const AcPreset* preset = ac_preset_registry_find_by_command(command); preset != nullptr) {
        *out_intent = preset->state;
        return true;
    }

    if (parse_fixed_temp_alias(command, out_intent)) {
        return true;
    }

    const struct {
        const char* old_command;
        AcPresetId preset_id;
    } aliases[] = {
        {"AC_COOL_26_FAN1", AC_PRESET_COOL_26_FAN1},
        {"AC_COOL_26_FAN2", AC_PRESET_COOL_26_FAN2},
        {"AC_COOL_26_FAN3", AC_PRESET_COOL_26_FAN3},
        {"AC_COOL_26_FAN5", AC_PRESET_COOL_26_FAN5},
        {"AC_SET_COOL_26_FAN1", AC_PRESET_COOL_26_FAN1},
        {"AC_SET_COOL_26_FAN2", AC_PRESET_COOL_26_FAN2},
        {"AC_SET_COOL_26_FAN3", AC_PRESET_COOL_26_FAN3},
        {"AC_SET_COOL_26_FAN5", AC_PRESET_COOL_26_FAN5},
        {"AC_SET_MODE_COOL", AC_PRESET_COOL_MODE},
        {"AC_SET_MODE_DRY", AC_PRESET_DRY_MODE},
        {"AC_SET_MODE_FAN", AC_PRESET_FAN_MODE},
        {"AC_SET_MODE_HEAT", AC_PRESET_HEAT_MODE},
    };

    for (const auto& alias : aliases) {
        if (strcmp(command, alias.old_command) == 0) {
            if (const AcPreset* alias_preset = ac_preset_registry_find_by_id(alias.preset_id); alias_preset != nullptr) {
                *out_intent = alias_preset->state;
                return true;
            }
        }
    }

    return false;
}
