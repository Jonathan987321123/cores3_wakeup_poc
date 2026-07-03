#include "ac_temperature_registry.h"

#include <stdio.h>
#include <string.h>

namespace {

static const AcTemperatureAssetRecord kTemperatureAssets[] = {
    {AC_PRESET_COOL_18_AUTO, "AC_SET_COOL_18", "AC_SET_COOL_18_AUTO", "COOL 18 AUTO", AC_MODE_COOL, 18, AC_TEMPERATURE_ASSET_DISABLED},
    {AC_PRESET_COOL_19_AUTO, "AC_SET_COOL_19", "AC_SET_COOL_19_AUTO", "COOL 19 AUTO", AC_MODE_COOL, 19, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_NONE, "AC_SET_COOL_20", nullptr, nullptr, AC_MODE_COOL, 20, AC_TEMPERATURE_ASSET_RECAPTURE_REQUIRED},
    {AC_PRESET_COOL_21_AUTO, "AC_SET_COOL_21", "AC_SET_COOL_21_AUTO", "COOL 21 AUTO", AC_MODE_COOL, 21, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_COOL_22_AUTO, "AC_SET_COOL_22", "AC_SET_COOL_22_AUTO", "COOL 22 AUTO", AC_MODE_COOL, 22, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_COOL_23_AUTO, "AC_SET_COOL_23", "AC_SET_COOL_23_AUTO", "COOL 23 AUTO", AC_MODE_COOL, 23, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_COOL_24_AUTO, "AC_SET_COOL_24", "AC_SET_COOL_24_AUTO", "COOL 24 AUTO", AC_MODE_COOL, 24, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_COOL_25_AUTO, "AC_SET_COOL_25", "AC_SET_COOL_25_AUTO", "COOL 25 AUTO", AC_MODE_COOL, 25, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_COOL_26_AUTO, "AC_SET_COOL_26", "AC_SET_COOL_26_AUTO", "COOL 26 AUTO", AC_MODE_COOL, 26, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_COOL_27_AUTO, "AC_SET_COOL_27", "AC_SET_COOL_27_AUTO", "COOL 27 AUTO", AC_MODE_COOL, 27, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_COOL_28_AUTO, "AC_SET_COOL_28", "AC_SET_COOL_28_AUTO", "COOL 28 AUTO", AC_MODE_COOL, 28, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_NONE, "AC_SET_COOL_29", nullptr, nullptr, AC_MODE_COOL, 29, AC_TEMPERATURE_ASSET_RECAPTURE_REQUIRED},
    {AC_PRESET_NONE, "AC_SET_COOL_30", nullptr, nullptr, AC_MODE_COOL, 30, AC_TEMPERATURE_ASSET_RECAPTURE_REQUIRED},
    {AC_PRESET_HEAT_18_AUTO, "AC_SET_HEAT_18", "AC_SET_HEAT_18_AUTO", "HEAT 18 AUTO", AC_MODE_HEAT, 18, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_19_AUTO, "AC_SET_HEAT_19", "AC_SET_HEAT_19_AUTO", "HEAT 19 AUTO", AC_MODE_HEAT, 19, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_20_AUTO, "AC_SET_HEAT_20", "AC_SET_HEAT_20_AUTO", "HEAT 20 AUTO", AC_MODE_HEAT, 20, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_21_AUTO, "AC_SET_HEAT_21", "AC_SET_HEAT_21_AUTO", "HEAT 21 AUTO", AC_MODE_HEAT, 21, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_22_AUTO, "AC_SET_HEAT_22", "AC_SET_HEAT_22_AUTO", "HEAT 22 AUTO", AC_MODE_HEAT, 22, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_23_AUTO, "AC_SET_HEAT_23", "AC_SET_HEAT_23_AUTO", "HEAT 23 AUTO", AC_MODE_HEAT, 23, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_24_AUTO, "AC_SET_HEAT_24", "AC_SET_HEAT_24_AUTO", "HEAT 24 AUTO", AC_MODE_HEAT, 24, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_25_AUTO, "AC_SET_HEAT_25", "AC_SET_HEAT_25_AUTO", "HEAT 25 AUTO", AC_MODE_HEAT, 25, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_26_AUTO, "AC_SET_HEAT_26", "AC_SET_HEAT_26_AUTO", "HEAT 26 AUTO", AC_MODE_HEAT, 26, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_27_AUTO, "AC_SET_HEAT_27", "AC_SET_HEAT_27_AUTO", "HEAT 27 AUTO", AC_MODE_HEAT, 27, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_28_AUTO, "AC_SET_HEAT_28", "AC_SET_HEAT_28_AUTO", "HEAT 28 AUTO", AC_MODE_HEAT, 28, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_29_AUTO, "AC_SET_HEAT_29", "AC_SET_HEAT_29_AUTO", "HEAT 29 AUTO", AC_MODE_HEAT, 29, AC_TEMPERATURE_ASSET_VERIFIED},
    {AC_PRESET_HEAT_30_AUTO, "AC_SET_HEAT_30", "AC_SET_HEAT_30_AUTO", "HEAT 30 AUTO", AC_MODE_HEAT, 30, AC_TEMPERATURE_ASSET_VERIFIED},
};

static const AcTemperatureAssetRecord* find_by_command_internal(const char* command) {
    if (command == nullptr) {
        return nullptr;
    }

    for (const AcTemperatureAssetRecord& asset : kTemperatureAssets) {
        if (strcmp(command, asset.base_command) == 0) {
            return &asset;
        }
        if (asset.preset_command != nullptr && strcmp(command, asset.preset_command) == 0) {
            return &asset;
        }
    }

    return nullptr;
}

static bool build_verified_preset(const AcTemperatureAssetRecord& asset, AcPreset* out_preset) {
    if (out_preset == nullptr || asset.status != AC_TEMPERATURE_ASSET_VERIFIED ||
        asset.preset_id == AC_PRESET_NONE || asset.preset_command == nullptr || asset.label == nullptr) {
        return false;
    }

    *out_preset = {
        asset.preset_id,
        asset.preset_command,
        asset.label,
        {true, true, true, asset.temp_c, asset.mode, AC_FAN_AUTO, false, false, asset.preset_id},
    };
    return true;
}

}  // namespace

const AcTemperatureAssetRecord* ac_temperature_registry_find_by_id(AcPresetId id) {
    for (const AcTemperatureAssetRecord& asset : kTemperatureAssets) {
        if (asset.preset_id == id) {
            return &asset;
        }
    }
    return nullptr;
}

const AcTemperatureAssetRecord* ac_temperature_registry_find_by_command(const char* command) {
    return find_by_command_internal(command);
}

const AcTemperatureAssetRecord* ac_temperature_registry_find_verified_by_mode_temp(AcMode mode, uint8_t temp_c) {
    for (const AcTemperatureAssetRecord& asset : kTemperatureAssets) {
        if (asset.mode == mode && asset.temp_c == temp_c && asset.status == AC_TEMPERATURE_ASSET_VERIFIED) {
            return &asset;
        }
    }
    return nullptr;
}

bool ac_temperature_registry_build_verified_preset_by_id(AcPresetId id, AcPreset* out_preset) {
    const AcTemperatureAssetRecord* asset = ac_temperature_registry_find_by_id(id);
    return asset != nullptr && build_verified_preset(*asset, out_preset);
}

bool ac_temperature_registry_build_verified_preset_by_command(const char* command, AcPreset* out_preset) {
    const AcTemperatureAssetRecord* asset = find_by_command_internal(command);
    return asset != nullptr && build_verified_preset(*asset, out_preset);
}

AcTemperatureAssetStatus ac_temperature_registry_get_status(AcPresetId id) {
    if (const AcTemperatureAssetRecord* asset = ac_temperature_registry_find_by_id(id); asset != nullptr) {
        return asset->status;
    }
    return AC_TEMPERATURE_ASSET_RECAPTURE_REQUIRED;
}
