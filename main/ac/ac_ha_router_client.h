#pragma once

#include "ac_control_minimal.h"

bool ac_ha_router_route_voice_intent(const char* device_id, const char* command, const char* text, const AcIntent* intent);
