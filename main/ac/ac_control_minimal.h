#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver/gpio.h"

enum AcMode {
    AC_MODE_UNKNOWN = 0,
    AC_MODE_COOL,
    AC_MODE_DRY,
    AC_MODE_HEAT,
    AC_MODE_FAN,
};

enum AcFan {
    AC_FAN_UNKNOWN = 0,
    AC_FAN_AUTO,
    AC_FAN_1,
    AC_FAN_2,
    AC_FAN_3,
    AC_FAN_5,
};

enum AcPresetId {
    AC_PRESET_NONE = 0,
    AC_PRESET_POWER_ON,
    AC_PRESET_POWER_OFF,
    AC_PRESET_COOL_25_AUTO,
    AC_PRESET_COOL_26_AUTO,
    AC_PRESET_COOL_27_AUTO,
    AC_PRESET_COOL_26_FAN1,
    AC_PRESET_COOL_26_FAN2,
    AC_PRESET_COOL_26_FAN3,
    AC_PRESET_COOL_26_FAN5,
    AC_PRESET_COOL_SLEEP_ON,
    AC_PRESET_HEAT_26_FAN1,
    AC_PRESET_HEAT_26_FAN2,
    AC_PRESET_HEAT_26_FAN3,
    AC_PRESET_HEAT_26_FAN5,
    AC_PRESET_COOL_MODE,
    AC_PRESET_DRY_MODE,
    AC_PRESET_FAN_MODE,
    AC_PRESET_HEAT_MODE,
    AC_PRESET_HEAT_SLEEP_ON,
    AC_PRESET_TEMP_UP_STATE,
    AC_PRESET_TEMP_DOWN_STATE,
    AC_PRESET_COOL_18_AUTO,
    AC_PRESET_COOL_19_AUTO,
    AC_PRESET_COOL_21_AUTO,
    AC_PRESET_COOL_22_AUTO,
    AC_PRESET_COOL_23_AUTO,
    AC_PRESET_COOL_24_AUTO,
    AC_PRESET_COOL_28_AUTO,
    AC_PRESET_HEAT_18_AUTO,
    AC_PRESET_HEAT_19_AUTO,
    AC_PRESET_HEAT_20_AUTO,
    AC_PRESET_HEAT_21_AUTO,
    AC_PRESET_HEAT_22_AUTO,
    AC_PRESET_HEAT_23_AUTO,
    AC_PRESET_HEAT_24_AUTO,
    AC_PRESET_HEAT_25_AUTO,
    AC_PRESET_HEAT_26_AUTO,
    AC_PRESET_HEAT_27_AUTO,
    AC_PRESET_HEAT_28_AUTO,
    AC_PRESET_HEAT_29_AUTO,
    AC_PRESET_HEAT_30_AUTO,
};

struct AcIntent {
    bool power_set;
    bool power_on;
    bool temp_set;
    uint8_t temp_c;
    AcMode mode;
    AcFan fan;
    bool sleep_set;
    bool sleep_on;
    AcPresetId preset_hint;
};

struct AcProfile {
    const char* name;
    gpio_num_t ir_gpio;
    uint8_t temp_default_c;
    uint8_t temp_min_c;
    uint8_t temp_max_c;
    bool supports_sleep;
};

struct AcPreset {
    AcPresetId id;
    const char* command;
    const char* label;
    AcIntent state;
};

struct AcStateSnapshot {
    AcIntent last_intent;
    AcPresetId last_preset_id;
};

void ac_control_init(const AcProfile& profile);
bool ac_control_parse_command(const char* command, AcIntent* out_intent);
bool ac_control_execute_intent(const AcIntent& intent, char* out_text, size_t out_text_len);
AcStateSnapshot ac_control_get_state(void);
