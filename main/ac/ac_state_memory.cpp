#include "ac_state_memory.h"

namespace {

static constexpr uint8_t kTempDefaultC = 25;

static AcStateSnapshot g_state = {
    .last_intent = {
        .power_set = false,
        .power_on = false,
        .temp_set = true,
        .temp_c = kTempDefaultC,
        .mode = AC_MODE_COOL,
        .fan = AC_FAN_AUTO,
        .sleep_set = false,
        .sleep_on = false,
        .preset_hint = AC_PRESET_NONE,
    },
    .last_preset_id = AC_PRESET_NONE,
};

AcIntent merge_state_after_success(const AcIntent& previous, const AcIntent& current, AcPresetId preset_id) {
    AcIntent merged = previous;
    merged.preset_hint = preset_id;

    if (current.power_set) {
        merged.power_set = true;
        merged.power_on = current.power_on;
    }
    if (current.temp_set) {
        merged.temp_set = true;
        merged.temp_c = current.temp_c;
    }
    if (current.mode != AC_MODE_UNKNOWN) {
        merged.mode = current.mode;
    }
    if (current.fan != AC_FAN_UNKNOWN) {
        merged.fan = current.fan;
    }
    if (current.sleep_set) {
        merged.sleep_set = true;
        merged.sleep_on = current.sleep_on;
    } else if (preset_id != AC_PRESET_POWER_ON && preset_id != AC_PRESET_POWER_OFF) {
        merged.sleep_set = false;
        merged.sleep_on = false;
    }

    return merged;
}

}  // namespace

void ac_state_memory_init(const AcProfile& profile) {
    g_state.last_intent = {
        .power_set = false,
        .power_on = false,
        .temp_set = true,
        .temp_c = profile.temp_default_c,
        .mode = AC_MODE_COOL,
        .fan = AC_FAN_AUTO,
        .sleep_set = false,
        .sleep_on = false,
        .preset_hint = AC_PRESET_NONE,
    };
    g_state.last_preset_id = AC_PRESET_NONE;
}

void ac_state_memory_apply_success(const AcIntent& intent, AcPresetId preset_id) {
    g_state.last_intent = merge_state_after_success(g_state.last_intent, intent, preset_id);
    g_state.last_preset_id = preset_id;
}

AcStateSnapshot ac_state_memory_get(void) {
    return g_state;
}

const char* ac_state_memory_mode_name(AcMode mode) {
    switch (mode) {
        case AC_MODE_COOL: return "cool";
        case AC_MODE_DRY: return "dry";
        case AC_MODE_HEAT: return "heat";
        case AC_MODE_FAN: return "fan";
        default: return "unknown";
    }
}

const char* ac_state_memory_fan_name(AcFan fan) {
    switch (fan) {
        case AC_FAN_AUTO: return "auto";
        case AC_FAN_1: return "fan1";
        case AC_FAN_2: return "fan2";
        case AC_FAN_3: return "fan3";
        case AC_FAN_5: return "fan5";
        default: return "unknown";
    }
}
