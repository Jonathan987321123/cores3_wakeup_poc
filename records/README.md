# CoreS3 Wake-up Word PoC Stage Records / CoreS3 唤醒词 PoC 阶段记录

This folder documents the development stages of the CoreS3 Wake-up Word PoC project.
本文件夹记录 CoreS3 唤醒词 PoC 项目的开发阶段。

## Stage Order / 阶段顺序

| # | Stage | 阶段 | Status |
|---|-------|------|--------|
| 01 | [Wake Word Framework Setup](01-wake-word-framework.md) | 唤醒词框架搭建 | ✅ Complete |
| 02 | [MQTT Bridge Implementation](02-mqtt-bridge.md) | MQTT 桥接实现 | ✅ Complete |
| 03 | [AC Control Integration](03-ac-control.md) | 空调控制集成 | ✅ Complete |
| 04 | [TV Control Integration](04-tv-control.md) | 电视控制集成 | ✅ Complete |
| 05 | [HA Discovery Integration](05-ha-discovery.md) | HA 自动发现集成 | ✅ Complete |
| 06 | [Voice Intent Processing](06-voice-intent.md) | 语音意图处理 | ✅ Complete |
| 07 | [Away Mode Implementation](07-away-mode.md) | 离家模式实现 | 🔄 In Progress |
| 08 | [Cloud Voice Integration](08-cloud-voice.md) | 云端语音集成 | 🔄 In Progress |

## Reading Rule / 阅读方式

- Read from top to bottom / 按顺序阅读
- Each file is one stage / 每个文件对应一个阶段
- Later stages build on earlier ones / 后面的阶段建立在前面的基础上

## Stage Summary / 阶段摘要

### Stage 01 - Wake Word Framework Setup / 唤醒词框架搭建

Establishing the ESP-SR wake word detection framework on CoreS3.

**Key points:**
- ESP-SR framework integration / ESP-SR 框架集成
- Wake word model selection / 唤醒词模型选择
- Threshold tuning / 阈值调优
- Audio pipeline setup / 音频管道设置

### Stage 02 - MQTT Bridge Implementation / MQTT 桥接实现

Building the MQTT bridge for Home Assistant integration.

**Key points:**
- MQTT client setup / MQTT 客户端设置
- Topic structure design / Topic 结构设计
- Message format definition / 消息格式定义
- Connection management / 连接管理

### Stage 03 - AC Control Integration / 空调控制集成

Integrating air conditioner IR control with voice commands.

**Key points:**
- AC IR protocol analysis / 空调 IR 协议分析
- Temperature preset registry / 温度预设注册表
- State persistence with NVS / 使用 NVS 持久化状态
- Mode switching logic / 模式切换逻辑

### Stage 04 - TV Control Integration / 电视控制集成

Integrating TV IR control for voice commands.

**Key points:**
- TV IR code mapping / 电视 IR 码映射
- Power, volume, source control / 电源、音量、源控制
- HA entity mapping / HA 实体映射

### Stage 05 - HA Discovery Integration / HA 自动发现集成

Implementing Home Assistant MQTT Discovery protocol.

**Key points:**
- Discovery payload format / 发现 payload 格式
- Entity registration / 实体注册
- State reporting / 状态上报
- Command subscription / 命令订阅

### Stage 06 - Voice Intent Processing / 语音意图处理

Processing voice commands into device actions.

**Key points:**
- Intent parsing / 意图解析
- Command routing / 命令路由
- Response generation / 响应生成
- Error handling / 错误处理

### Stage 07 - Away Mode Implementation / 离家模式实现 🔄

Adding away/leaving-home mode functionality.

**Key points:**
- Presence detection / 在场检测
- Automated shutdown / 自动关闭设备
- Notification system / 通知系统

### Stage 08 - Cloud Voice Integration / 云端语音集成 🔄

Connecting to cloud-based ASR services.

**Key points:**
- API integration / API 集成
- Fallback mechanisms / 降级机制
- Latency optimization / 延迟优化
