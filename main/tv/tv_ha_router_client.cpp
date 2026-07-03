#include "tv_ha_router_client.h"

#include "mqtt_bridge.h"

bool tv_ha_router_route_voice_intent(const char* device_id, const char* command, const char* text) {
    return mqtt_bridge_publish_device_intent(device_id, "tv", command, text);
}
