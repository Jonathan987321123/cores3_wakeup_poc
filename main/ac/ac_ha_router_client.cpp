#include "ac_ha_router_client.h"

#include "mqtt_bridge.h"

bool ac_ha_router_route_voice_intent(const char* device_id, const char* command, const char* text, const AcIntent* intent) {
    return mqtt_bridge_publish_voice_intent(device_id, command, text, intent);
}
