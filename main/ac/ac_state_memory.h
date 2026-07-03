#pragma once

#include "ac_control_minimal.h"

void ac_state_memory_init(const AcProfile& profile);
void ac_state_memory_apply_success(const AcIntent& intent, AcPresetId preset_id);
AcStateSnapshot ac_state_memory_get(void);

const char* ac_state_memory_mode_name(AcMode mode);
const char* ac_state_memory_fan_name(AcFan fan);
