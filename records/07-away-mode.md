# Stage 07 - Away Mode Implementation / 阶段 07 - 离家模式实现

## English

### Overview

Away mode (离家模式) is a key feature for home automation. When enabled, it automatically controls devices based on presence detection and schedules.

### What Away Mode Does

| Scenario | Action |
|----------|--------|
| User leaves home | Turn off AC, TV, lights |
| User returns home | Restore previous states |
| Away timeout | Safety shutdown all devices |
| Emergency | Quick all-off button |

### Presence Detection

The system detects presence through:

1. **Phone proximity** (via HA device tracker)
   - WiFi connection to home network
   - Bluetooth LE beacon
   - GPS geofencing

2. **Manual toggle**
   - Voice command: "我出门了" / "I'm leaving"
   - Voice command: "我回来了" / "I'm back"
   - HA automation trigger

3. **Schedule-based**
   - Work day schedule (Mon-Fri 9:00-18:00)
   - Weekend schedule

### State Management

Before activating away mode, the system saves current device states:

```cpp
struct AwayModeSnapshot {
    bool ac_power;
    uint8_t ac_temperature;
    bool tv_power;
    uint8_t tv_volume;
    time_t timestamp;
};
```

### Voice Commands

| Command | Action |
|---------|--------|
| 我出门了 / I'm leaving | Activate away mode |
| 我回来了 / I'm back | Deactivate away mode |
| 全部关闭 / Turn everything off | Emergency all-off |
| 全部打开 / Turn everything on | Restore all devices |

### HA Automation Example

```yaml
automation:
  - alias: "Away Mode - User Leaves"
    trigger:
      - platform: state
        entity_id: person.jonathan
        to: not_home
    action:
      - service: mqtt.publish
        data:
          topic: cores3/bedroom/away/activate
          payload: '{"reason": "user_left"}'

  - alias: "Away Mode - User Returns"
    trigger:
      - platform: state
        entity_id: person.jonathan
        to: home
    action:
      - service: mqtt.publish
        data:
          topic: cores3/bedroom/away/deactivate
          payload: '{"reason": "user_returned"}'
```

---

## 中文

### 概述

离家模式是家庭自动化的关键功能。启用后，它会根据在场检测和时间表自动控制设备。

### 离家模式做什么

| 场景 | 操作 |
|------|------|
| 用户离家 | 关闭空调、电视、灯 |
| 用户回家 | 恢复之前的状态 |
| 离家超时 | 安全关闭所有设备 |
| 紧急情况 | 快速全关按钮 |

### 在场检测

系统通过以下方式检测在场：

1. **手机靠近**（通过 HA 设备追踪器）
   - WiFi 连接到家庭网络
   - 蓝牙 LE 信标
   - GPS 地理围栏

2. **手动切换**
   - 语音命令："我出门了"
   - 语音命令："我回来了"
   - HA 自动化触发

3. **基于时间表**
   - 工作日时间表（周一至周五 9:00-18:00）
   - 周末时间表

### 状态管理

激活离家模式前，系统保存当前设备状态：

```cpp
struct AwayModeSnapshot {
    bool ac_power;
    uint8_t ac_temperature;
    bool tv_power;
    uint8_t tv_volume;
    time_t timestamp;
};
```

### 语音命令

| 命令 | 操作 |
|------|------|
| 我出门了 | 激活离家模式 |
| 我回来了 | 停用离家模式 |
| 全部关闭 | 紧急全关 |
| 全部打开 | 恢复所有设备 |

### HA 自动化示例

```yaml
automation:
  - alias: "离家模式 - 用户离开"
    trigger:
      - platform: state
        entity_id: person.jonathan
        to: not_home
    action:
      - service: mqtt.publish
        data:
          topic: cores3/bedroom/away/activate
          payload: '{"reason": "user_left"}'

  - alias: "离家模式 - 用户返回"
    trigger:
      - platform: state
        entity_id: person.jonathan
        to: home
    action:
      - service: mqtt.publish
        data:
          topic: cores3/bedroom/away/deactivate
          payload: '{"reason": "user_returned"}'
```
