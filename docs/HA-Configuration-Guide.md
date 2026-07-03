# Home Assistant MQTT Configuration Guide / Home Assistant MQTT 配置指南

## English

### Overview

This guide explains how to configure Home Assistant to work with the CoreS3 Wake-up Word PoC system.

### Prerequisites

1. Home Assistant installed and running
2. MQTT broker installed (Mosquitto recommended)
3. CoreS3 device with firmware flashed
4. Network connectivity between HA and CoreS3

### Step 1: Configure MQTT in Home Assistant

Add to your `configuration.yaml`:

```yaml
mqtt:
  broker: 192.168.x.x       # Your MQTT broker IP
  port: 1883
  username: mqtt_user      # Optional
  password: mqtt_password    # Optional
  discovery: true
  discovery_prefix: homeassistant
```

### Step 2: Configure Home Assistant Integration

#### Enable MQTT Integration
1. Go to **Settings** → **Devices & Services**
2. Click **Add Integration**
3. Search for **MQTT**
4. Configure broker settings

#### Create Zones for Presence Detection
```yaml
homeassistant:
  zones:
    - name: Home
      latitude: 31.2304
      longitude: 121.4737
      radius: 100
      icon: mdi:home
```

### Step 3: Create Automations

#### AC Control Automation
```yaml
automation:
  - alias: "Voice - Set AC Temperature"
    trigger:
      platform: mqtt
      topic: cores3/bedroom/ir/ac/state
    action:
      - service: notify.persistent_notification
        data:
          title: "AC Update"
          message: "Temperature set to {{ trigger.payload_json.temperature }}°C"
```

#### Away Mode Automation
```yaml
automation:
  - alias: "Away Mode - Auto Shutdown"
    trigger:
      - platform: state
        entity_id: binary_sensor.away_mode
        to: 'on'
    action:
      - service: switch.turn_off
        target:
          entity_id:
            - switch.bedroom_ac
            - switch.bedroom_tv
```

### Step 4: Dashboard Configuration

Create a dashboard card for CoreS3:

```yaml
type: entities
entities:
  - entity: climate.bedroom_ac
  - entity: switch.bedroom_tv
  - entity: sensor.cores3_status
  - entity: binary_sensor.away_mode
title: CoreS3 Control Panel
```

---

## 中文

### 概述

本指南说明如何配置 Home Assistant 以与 CoreS3 唤醒词 PoC 系统配合工作。

### 前置要求

1. Home Assistant 已安装并运行
2. MQTT broker 已安装（推荐 Mosquitto）
3. CoreS3 设备已烧录固件
4. HA 和 CoreS3 之间网络连通

### 步骤 1: 在 Home Assistant 中配置 MQTT

添加到 `configuration.yaml`:

```yaml
mqtt:
  broker: 192.168.x.x       # 您的 MQTT broker IP
  port: 1883
  username: mqtt_user      # 可选
  password: mqtt_password  # 可选
  discovery: true
  discovery_prefix: homeassistant
```

### 步骤 2: 配置 Home Assistant 集成

#### 启用 MQTT 集成
1. 进入 **设置** → **设备与服务**
2. 点击 **添加集成**
3. 搜索 **MQTT**
4. 配置 broker 设置

#### 创建在场检测区域
```yaml
homeassistant:
  zones:
    - name: 家
      latitude: 31.2304
      longitude: 121.4737
      radius: 100
      icon: mdi:home
```

### 步骤 3: 创建自动化

#### 空调控制自动化
```yaml
automation:
  - alias: "语音 - 设置空调温度"
    trigger:
      platform: mqtt
      topic: cores3/bedroom/ir/ac/state
    action:
      - service: notify.persistent_notification
        data:
          title: "空调更新"
          message: "温度已设置为 {{ trigger.payload_json.temperature }}°C"
```

#### 离家模式自动化
```yaml
automation:
  - alias: "离家模式 - 自动关闭"
    trigger:
      - platform: state
        entity_id: binary_sensor.away_mode
        to: 'on'
    action:
      - service: switch.turn_off
        target:
          entity_id:
            - switch.bedroom_ac
            - switch.bedroom_tv
```

### 步骤 4: 仪表盘配置

为 CoreS3 创建仪表盘卡片：

```yaml
type: entities
entities:
  - entity: climate.bedroom_ac
  - entity: switch.bedroom_tv
  - entity: sensor.cores3_status
  - entity: binary_sensor.away_mode
title: CoreS3 控制面板
```

## MQTT Topic Reference / MQTT Topic 参考

### Device Topics / 设备 Topic

| Topic | Direction | Description |
|-------|-----------|-------------|
| `cores3/<device_id>/status` | ← Device | Device status |
| `cores3/<device_id>/command` | → Device | General commands |
| `cores3/<device_id>/wakeup` | → Device | Wake-up trigger |

### AC Topics / 空调 Topic

| Topic | Direction | Description |
|-------|-----------|-------------|
| `cores3/<room>/ir/ac/command` | → Device | AC command |
| `cores3/<room>/ir/ac/state` | ← Device | AC state report |
| `cores3/<room>/ir/ac/temperature` | → Device | Temperature set |

### TV Topics / 电视 Topic

| Topic | Direction | Description |
|-------|-----------|-------------|
| `cores3/<room>/ir/tv/command` | → Device | TV command |
| `cores3/<room>/ir/tv/state` | ← Device | TV state report |

### Away Mode Topics / 离家模式 Topic

| Topic | Direction | Description |
|-------|-----------|-------------|
| `cores3/<device_id>/away/activate` | → Device | Activate away mode |
| `cores3/<device_id>/away/deactivate` | → Device | Deactivate away mode |
| `cores3/<device_id>/away/status` | ← Device | Away mode status |
