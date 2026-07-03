#include "ac_preset_registry.h"
#include "ac_state_memory.h"
#include "ac_temperature_registry.h"

#include <string.h>

namespace {

static bool preset_matches_intent(const AcPreset& preset, const AcIntent& intent) {
    if (intent.power_set && preset.state.power_on != intent.power_on) {
        return false;
    }
    if (intent.temp_set && preset.state.temp_c != intent.temp_c) {
        return false;
    }
    if (intent.mode != AC_MODE_UNKNOWN && preset.state.mode != intent.mode) {
        return false;
    }
    if (intent.fan != AC_FAN_UNKNOWN && preset.state.fan != intent.fan) {
        return false;
    }
    if (intent.sleep_set && preset.state.sleep_on != intent.sleep_on) {
        return false;
    }
    return true;
}

static const AcPulsePreset kPulsePresets[] = {
    {{
         AC_PRESET_COOL_26_AUTO,
         "AC_SET_COOL_26_AUTO",
         "COOL 26 AUTO",
         {true, true, true, 26, AC_MODE_COOL, AC_FAN_AUTO, false, false, AC_PRESET_COOL_26_AUTO},
     },
     0x250200B09ULL,
     0x270200B09ULL},
    {{
         AC_PRESET_COOL_27_AUTO,
         "AC_SET_COOL_27_AUTO",
         "COOL 27 AUTO",
         {true, true, true, 27, AC_MODE_COOL, AC_FAN_AUTO, false, false, AC_PRESET_COOL_27_AUTO},
     },
     0x250200C09ULL,
     0x270200C09ULL},
    {{
         AC_PRESET_COOL_26_FAN1,
         "AC_FAN_1",
         "COOL 26 FAN1",
         {true, true, true, 26, AC_MODE_COOL, AC_FAN_1, false, false, AC_PRESET_COOL_26_FAN1},
     },
     0x250200A19ULL,
     0x270200A19ULL},
    {{
         AC_PRESET_COOL_26_FAN2,
         "AC_FAN_2",
         "COOL 26 FAN2",
         {true, true, true, 26, AC_MODE_COOL, AC_FAN_2, false, false, AC_PRESET_COOL_26_FAN2},
     },
     0x250200A29ULL,
     0x270200A29ULL},
    {{
         AC_PRESET_COOL_26_FAN3,
         "AC_FAN_3",
         "COOL 26 FAN3",
         {true, true, true, 26, AC_MODE_COOL, AC_FAN_3, false, false, AC_PRESET_COOL_26_FAN3},
     },
     0x250200A39ULL,
     0x270200A39ULL},
    {{
         AC_PRESET_COOL_26_FAN5,
         "AC_FAN_5",
         "COOL 26 FAN5",
         {true, true, true, 26, AC_MODE_COOL, AC_FAN_5, false, false, AC_PRESET_COOL_26_FAN5},
     },
     0x250200A59ULL,
     0x270200A59ULL},
    {{
         AC_PRESET_COOL_SLEEP_ON,
         "AC_SLEEP_ON_COOL",
         "COOL SLEEP ON",
         {true, true, false, 0, AC_MODE_COOL, AC_FAN_UNKNOWN, true, true, AC_PRESET_COOL_SLEEP_ON},
     },
     0x250200A89ULL,
     0x270200A89ULL},
    {{
         AC_PRESET_HEAT_26_FAN1,
         "AC_HEAT_26_FAN1",
         "HEAT 26 FAN1",
         {true, true, true, 26, AC_MODE_HEAT, AC_FAN_1, false, false, AC_PRESET_HEAT_26_FAN1},
     },
     0x2800200050200A1CULL,
     0x2800200050200A1CULL},
    {{
         AC_PRESET_HEAT_26_FAN2,
         "AC_HEAT_26_FAN2",
         "HEAT 26 FAN2",
         {true, true, true, 26, AC_MODE_HEAT, AC_FAN_2, false, false, AC_PRESET_HEAT_26_FAN2},
     },
     0x2800200050200A2CULL,
     0x2800200050200A2CULL},
    {{
         AC_PRESET_HEAT_26_FAN3,
         "AC_HEAT_26_FAN3",
         "HEAT 26 FAN3",
         {true, true, true, 26, AC_MODE_HEAT, AC_FAN_3, false, false, AC_PRESET_HEAT_26_FAN3},
     },
     0x2800200050200A3CULL,
     0x2800200050200A3CULL},
    {{
         AC_PRESET_HEAT_26_FAN5,
         "AC_HEAT_26_FAN5",
         "HEAT 26 FAN5",
         {true, true, true, 26, AC_MODE_HEAT, AC_FAN_5, false, false, AC_PRESET_HEAT_26_FAN5},
     },
     0x250200A5CULL,
     0x270200A5CULL},
    {{
         AC_PRESET_COOL_MODE,
         "AC_MODE_COOL",
         "MODE COOL",
         {true, true, false, 0, AC_MODE_COOL, AC_FAN_UNKNOWN, false, false, AC_PRESET_COOL_MODE},
     },
     0x250200A09ULL,
     0x270200A09ULL},
    {{
         AC_PRESET_DRY_MODE,
         "AC_MODE_DRY",
         "MODE DRY",
         {true, true, false, 0, AC_MODE_DRY, AC_FAN_UNKNOWN, false, false, AC_PRESET_DRY_MODE},
     },
     0x250200C1AULL,
     0x270200C1AULL},
    {{
         AC_PRESET_FAN_MODE,
         "AC_MODE_FAN",
         "MODE FAN",
         {true, true, false, 0, AC_MODE_FAN, AC_FAN_UNKNOWN, false, false, AC_PRESET_FAN_MODE},
     },
     0x250200A0BULL,
     0x270200A0BULL},
    {{
         AC_PRESET_HEAT_MODE,
         "AC_MODE_HEAT",
         "MODE HEAT",
         {true, true, false, 0, AC_MODE_HEAT, AC_FAN_UNKNOWN, false, false, AC_PRESET_HEAT_MODE},
     },
     0x250200A0CULL,
     0x270200A0CULL},
    {{
         AC_PRESET_HEAT_SLEEP_ON,
         "AC_SLEEP_ON_HEAT",
         "HEAT SLEEP ON",
         {true, true, false, 0, AC_MODE_HEAT, AC_FAN_UNKNOWN, true, true, AC_PRESET_HEAT_SLEEP_ON},
     },
     0x250200A8CULL,
     0x270200A8CULL},
};

static const AcPreset kPresets[] = {
    {
        AC_PRESET_POWER_ON,
        "AC_ON",
        "AC ON",
        {true, true, false, 0, AC_MODE_UNKNOWN, AC_FAN_UNKNOWN, false, false, AC_PRESET_POWER_ON},
    },
    {
        AC_PRESET_POWER_OFF,
        "AC_OFF",
        "AC OFF",
        {true, false, false, 0, AC_MODE_UNKNOWN, AC_FAN_UNKNOWN, false, false, AC_PRESET_POWER_OFF},
    },
    {
        AC_PRESET_TEMP_UP_STATE,
        "AC_TEMP_UP",
        "TEMP UP STATE",
        {true, true, true, 30, AC_MODE_FAN, AC_FAN_UNKNOWN, false, false, AC_PRESET_TEMP_UP_STATE},
    },
    {
        AC_PRESET_TEMP_DOWN_STATE,
        "AC_TEMP_DOWN",
        "TEMP DOWN STATE",
        {true, true, true, 25, AC_MODE_COOL, AC_FAN_AUTO, false, false, AC_PRESET_TEMP_DOWN_STATE},
    },
};

}  // namespace

const AcPreset* ac_preset_registry_find_by_id(AcPresetId id) {
    static AcPreset temp_preset = {};
    if (ac_temperature_registry_build_verified_preset_by_id(id, &temp_preset)) {
        return &temp_preset;
    }
    for (const AcPreset& preset : kPresets) {
        if (preset.id == id) {
            return &preset;
        }
    }
    for (const AcPulsePreset& preset : kPulsePresets) {
        if (preset.preset.id == id) {
            return &preset.preset;
        }
    }
    return nullptr;
}

const AcPreset* ac_preset_registry_find_by_command(const char* command) {
    if (command == nullptr) {
        return nullptr;
    }
    static AcPreset temp_preset = {};
    if (ac_temperature_registry_build_verified_preset_by_command(command, &temp_preset)) {
        return &temp_preset;
    }
    for (const AcPreset& preset : kPresets) {
        if (strcmp(command, preset.command) == 0) {
            return &preset;
        }
    }
    for (const AcPulsePreset& preset : kPulsePresets) {
        if (strcmp(command, preset.preset.command) == 0) {
            return &preset.preset;
        }
    }
    return nullptr;
}

const AcPulsePreset* ac_preset_registry_find_pulse_by_id(AcPresetId id) {
    for (const AcPulsePreset& preset : kPulsePresets) {
        if (preset.preset.id == id) {
            return &preset;
        }
    }
    return nullptr;
}

const AcPreset* ac_preset_registry_resolve_intent(const AcIntent& intent) {
    if (intent.preset_hint != AC_PRESET_NONE) {
        return ac_preset_registry_find_by_id(intent.preset_hint);
    }

    for (const AcPreset& preset : kPresets) {
        if (preset.id == AC_PRESET_TEMP_UP_STATE || preset.id == AC_PRESET_TEMP_DOWN_STATE) {
            continue;
        }
        if (preset_matches_intent(preset, intent)) {
            return &preset;
        }
    }

    for (const AcPulsePreset& preset : kPulsePresets) {
        if (preset_matches_intent(preset.preset, intent)) {
            return &preset.preset;
        }
    }

    return nullptr;
}
