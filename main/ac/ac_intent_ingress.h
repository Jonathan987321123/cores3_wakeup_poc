#pragma once

#include "ac_control_minimal.h"

bool ac_intent_ingress_parse_command(const AcProfile* profile, const char* command, AcIntent* out_intent);
