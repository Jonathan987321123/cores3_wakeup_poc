#pragma once

#include <stddef.h>
#include <stdint.h>

#include "ac_control_minimal.h"

struct AcRawAssetView {
    const uint16_t* raw;
    size_t len;
};

bool ac_temp_assets_find(AcPresetId id, AcRawAssetView* out_asset);
