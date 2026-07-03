#pragma once

#include "ac_control_minimal.h"

struct AcPulsePreset {
    AcPreset preset;
    uint64_t raw_data_1;
    uint64_t raw_data_2;
};

const AcPreset* ac_preset_registry_find_by_id(AcPresetId id);
const AcPreset* ac_preset_registry_find_by_command(const char* command);
const AcPulsePreset* ac_preset_registry_find_pulse_by_id(AcPresetId id);
const AcPreset* ac_preset_registry_resolve_intent(const AcIntent& intent);
