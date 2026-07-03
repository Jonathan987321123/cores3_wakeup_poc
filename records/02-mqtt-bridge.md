# Stage 02 - MQTT Bridge Implementation / 阶段 02 - MQTT 桥接实现

## English

### Overview

This stage establishes the MQTT bridge that connects CoreS3 with Home Assistant. The MQTT bridge is the backbone of the entire home automation system.

### What is the MQTT Bridge?

The MQTT bridge handles:
- Publishing device states to HA
- Subscribing to control commands from HA
- Managing connection lifecycle
- Handling reconnection logic

### Topic Structure

```
cores3/<device_id>/
├── voice/
│   ├── intent/         # Voice intent commands
│   └── status/         # Voice processing status
├── ir/
│   ├── <room>/execute/    # IR command execution
│   └── <room>/status/    # IR device status
├── sensor/
│   └── presence/      # Presence detection
└── config/            # Configuration topic
```

### Message Format

**State Report:**
```json
{
  "state": "on",
  "attributes": {
    "temperature": 26,
    "mode": "cool",
    "fan_speed": "auto"
  }
}
```

**Command:**
```json
{
  "command": "set_temperature",
  "value": 24
}
```

### Implementation Details

1. **Connection Setup**
   - Broker URI: configurable via menuconfig
   - Keep-alive: 60 seconds
   - Clean session: enabled

2. **QoS Levels**
   - State reports: QoS 1 (at least once)
   - Commands: QoS 0 (at most once)
   - Critical commands: QoS 2 (exactly once)

3. **Reconnection Strategy**
   - Initial delay: 1 second
   - Max delay: 60 seconds
   - Exponential backoff: 2x

---

## 中文

### 概述

本阶段建立 MQTT 桥接，连接 CoreS3 与 Home Assistant。MQTT 桥接是整个家庭自动化系统的支柱。

### 什么是 MQTT 桥接？

MQTT 桥接处理：
- 向 HA 发布设备状态
- 订阅来自 HA 的控制命令
- 管理连接生命周期
- 处理重连逻辑

### Topic 结构

```
cores3/<device_id>/
├── voice/
│   ├── intent/         # 语音意图命令
│   └── status/         # 语音处理状态
├── ir/
│   ├── <room>/execute/    # 红外命令执行
│   └── <room>/status/    # 红外设备状态
├── sensor/
│   └── presence/      # 在场检测
└── config/            # 配置 topic
```

### 消息格式

**状态上报：**
```json
{
  "state": "on",
  "attributes": {
    "temperature": 26,
    "mode": "cool",
    "fan_speed": "auto"
  }
}
```

**命令：**
```json
{
  "command": "set_temperature",
  "value": 24
}
```

### 实现细节

1. **连接设置**
   - Broker URI: 通过 menuconfig 配置
   - Keep-alive: 60 秒
   - Clean session: 启用

2. **QoS 级别**
   - 状态上报: QoS 1 (至少一次)
   - 命令: QoS 0 (最多一次)
   - 关键命令: QoS 2 (恰好一次)

3. **重连策略**
   - 初始延迟: 1 秒
   - 最大延迟: 60 秒
   - 指数退避: 2倍
