# Stage 03 - AC Control Integration / 阶段 03 - 空调控制集成

## English

### Overview

This stage integrates air conditioner control with the voice control system. The AC control module supports multiple AC brands through a unified interface.

### Supported AC Functions

| Function | Command | HA Entity |
|----------|---------|-----------|
| Power | on/off | switch |
| Temperature | 16-30°C | climate |
| Mode | cool/heat/dry/fan | climate |
| Fan Speed | auto/1-4 | climate |
| Swing | on/off | switch |

### Temperature Presets

Common temperature presets for quick voice commands:

| Voice Command | Temperature |
|---------------|-------------|
| 太热了 / Too hot | 24°C |
| 太冷了 / Too cold | 28°C |
| 制冷 / Cool down | 22°C |
| 制热 / Heat up | 26°C |
| 睡觉 / Sleep mode | 26°C + dry mode |

### State Persistence

AC state is persisted to NVS to survive reboots:

```cpp
// State stored in NVS
struct ACState {
    bool power;
    uint8_t temperature;
    ACMode mode;
    FanSpeed fan_speed;
    bool swing;
};
```

### IR Code Format

AC IR codes are sent in NEC extended format with manufacturer-specific timing.

---

## 中文

### 概述

本阶段将空调控制与语音控制系统集成。空调控制模块通过统一接口支持多个空调品牌。

### 支持的空调功能

| 功能 | 命令 | HA 实体 |
|------|------|---------|
| 电源 | on/off | switch |
| 温度 | 16-30°C | climate |
| 模式 | 制冷/制热/除湿/送风 | climate |
| 风速 | 自动/1-4档 | climate |
| 摆风 | on/off | switch |

### 温度预设

常用温度预设用于快速语音命令：

| 语音命令 | 温度 |
|----------|------|
| 太热了 | 24°C |
| 太冷了 | 28°C |
| 制冷 | 22°C |
| 制热 | 26°C |
| 睡觉 | 26°C + 除湿模式 |

### 状态持久化

空调状态持久化到 NVS 以跨重启保存：

```cpp
// 存储在 NVS 的状态
struct ACState {
    bool power;
    uint8_t temperature;
    ACMode mode;
    FanSpeed fan_speed;
    bool swing;
};
```

### 红外码格式

空调红外码以 NEC 扩展格式发送，具有特定制造商的时序。
