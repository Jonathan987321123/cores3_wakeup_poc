#include "ac_temp_assets.h"
#include "ac_temperature_registry.h"

namespace {

#include "generated_ac_temp_assets.inc"

}  // namespace

bool ac_temp_assets_find(AcPresetId id, AcRawAssetView* out_asset) {
    if (out_asset == nullptr) {
        return false;
    }

    if (ac_temperature_registry_get_status(id) != AC_TEMPERATURE_ASSET_VERIFIED) {
        return false;
    }

    for (const AcRawAsset& asset : kTemperatureRawAssets) {
        if (asset.id == id) {
            out_asset->raw = asset.raw;
            out_asset->len = asset.len;
            return true;
        }
    }

    return false;
}
