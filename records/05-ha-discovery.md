# Stage 05 - Home Assistant MQTT Discovery / 阶段 05 - HA MQTT 自动发现

## English

### Overview

Home Assistant's MQTT Discovery allows devices to auto-register themselves. This stage implements the discovery protocol for automatic entity creation.

### Discovery Topics

```
homeassistant/
├── switch/<device_id>_power/config
├── climate/<device_id>_ac/config
├── sensor/<device_id>_status/config
└── button/<device_id>_reboot/config
```

### Discovery Payloads

**AC Climate Entity:**
```json
{
  "name": "Bedroom AC",
  "unique_id": "cores3_bedroom_ac",
  "command_topic": "cores3/bedroom/ir/ac/command",
  "state_topic": "cores3/bedroom/ir/ac/state",
  "temperature_command_topic": "cores3/bedroom/ir/ac/temperature",
  "mode_command_topic": "cores3/bedroom/ir/ac/mode",
  "fan_mode_command_topic": "cores3/bedroom/ir/ac/fan",
  "modes": ["off", "cool", "heat", "dry", "fan_only"],
  "fan_modes": ["auto", "low", "medium", "high"],
  "min_temp": 16,
  "max_temp": 30,
  "temp_step": 1,
  "device": {
    "identifiers": ["cores3_bedroom"],
    "name": "CoreS3 Bedroom",
    "model": "M5Stack CoreS3",
    "manufacturer": "M5Stack"
  }
}
```

### Discovery Flow

1. Device boots and connects to MQTT broker
2. Device publishes discovery payloads to HA
3. HA receives and creates entities
4. Entities appear in HA UI automatically
5. Device reports state to HA periodically

---

## 中文

### 概述

Home Assistant 的 MQTT Discovery 允许设备自动注册。本阶段实现自动创建实体的发现协议。

### 发现 Topic

```
homeassistant/
├── switch/<device_id>_power/config
├── climate/<device_id>_ac/config
├── sensor/<device_id>_status/config
└── button/<device_id>_reboot/config
```

### 发现 Payload

**空调 Climate 实体：**
```json
{
  "name": "卧室空调",
  "unique_id": "cores3_bedroom_ac",
  "command_topic": "cores3/bedroom/ir/ac/command",
  "state_topic": "cores3/bedroom/ir/ac/state",
  "temperature_command_topic": "cores3/bedroom/ir/ac/temperature",
  "mode_command_topic": "cores3/bedroom/ir/ac/mode",
  "fan_mode_command_topic": "cores3/bedroom/ir/ac/fan",
  "modes": ["off", "cool", "heat", "dry", "fan_only"],
  "fan_modes": ["auto", "low", "medium", "high"],
  "min_temp": 16,
  "max_temp": 30,
  "temp_step": 1,
  "device": {
    "identifiers": ["cores3_bedroom"],
    "name": "CoreS3 卧室",
    "model": "M5Stack CoreS3",
    "manufacturer": "M5Stack"
  }
}
```

### 发现流程

1. 设备启动并连接 MQTT broker
2. 设备向 HA 发布发现 payload
3. HA 接收并创建实体
4. 实体自动出现在 HA UI 中
5. 设备定期向 HA 上报状态
