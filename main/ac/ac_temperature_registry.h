#pragma once

#include "ac_control_minimal.h"

enum AcTemperatureAssetStatus {
    AC_TEMPERATURE_ASSET_VERIFIED = 0,
    AC_TEMPERATURE_ASSET_DISABLED,
    AC_TEMPERATURE_ASSET_RECAPTURE_REQUIRED,
};

struct AcTemperatureAssetRecord {
    AcPresetId preset_id;
    const char* base_command;
    const char* preset_command;
    const char* label;
    AcMode mode;
    uint8_t temp_c;
    AcTemperatureAssetStatus status;
};

const AcTemperatureAssetRecord* ac_temperature_registry_find_by_id(AcPresetId id);
const AcTemperatureAssetRecord* ac_temperature_registry_find_by_command(const char* command);
const AcTemperatureAssetRecord* ac_temperature_registry_find_verified_by_mode_temp(AcMode mode, uint8_t temp_c);
bool ac_temperature_registry_build_verified_preset_by_id(AcPresetId id, AcPreset* out_preset);
bool ac_temperature_registry_build_verified_preset_by_command(const char* command, AcPreset* out_preset);
AcTemperatureAssetStatus ac_temperature_registry_get_status(AcPresetId id);
