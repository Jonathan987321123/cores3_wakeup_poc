# CoreS3 Home Smart Center Development Story / CoreS3 家庭智能中心开发故事

## English

### Why This Project

After completing the basic experiments with CoreS3 (documented in [from-zero-to-cores3](https://github.com/Jonathan987321123/from-zero-to-cores3)), I wanted to build a practical home automation system.

The core question was: **How to make CoreS3 not just a single device, but part of a complete home automation ecosystem?**

### System Architecture

The system is designed with clear role definitions:

```
CoreS3 = Mobile Voice Entry + Room IR Execution Node
Home Assistant = Home Hub + Automation Orchestration + Device Routing
Cloud Worker = ASR/NLU Voice Understanding Layer
```

### Two Parallel Development Lines

#### Line 1: Cloud Voice Line
- CoreS3 records, uploads to cloud, gets command back, executes locally via IR
- Suitable for TV, AC and other "speak and execute nearby devices immediately" scenarios

#### Line 2: HA MQTT Line
- CoreS3 connects to Home Assistant via MQTT
- HA handles automation and device orchestration
- Suitable for multi-device linkage, state management, and future home center expansion

### Technical Challenges

#### Challenge 1: Wake Word Detection
Wake word detection on embedded devices is challenging. Using ESP-SR framework with proper PSRAM configuration.

#### Challenge 2: MQTT Integration with HA
Home Assistant's MQTT Discovery protocol is powerful but requires careful payload format handling.

#### Challenge 3: IR Control Timing
AC IR codes have strict timing requirements. A few microseconds difference could mean the difference between "turn on" and "do nothing".

#### Challenge 4: State Persistence
AC state needs to persist across reboots to avoid "forgetting" the temperature setting.

### What I Learned

- **Clear role definition is essential**: CoreS3 should not try to do everything
- **HA is the brain, CoreS3 is the executor**: This separation of concerns makes the system maintainable
- **IR codes are execution layer assets**: They can be reused across different voice/cloud providers
- **NVS is your friend**: State persistence prevents unexpected behavior after reboots

### Current Achievements

| Device | Command | Status |
|--------|---------|--------|
| Bedroom AC | Cooling | ✅ |
| Bedroom AC | Heating | ✅ |
| Bedroom AC | Fan Speed | ✅ |
| Bedroom AC | Sleep Mode | ✅ |
| Bedroom TV | POWER/SOURCE/MENU/BACK/OK/MUTE/VOL | ✅ |
| Away Mode | Auto device shutdown | 🔄 In Progress |

### Current MQTT Topics

**Voice Intent Report:**
```
Topic: cores3/voice/intent
Payload: {"trace_id": "xxx", "source": "cores3_mobile", "room": "bedroom", "device_id": "bedroom_ac", "domain": "ac", "action": "AC_SET_COOL_26_AUTO", "text": "卧室空调制冷26度"}
```

**HA Execute Command:**
```
Topic: cores3/ir/bedroom/execute
Payload: {"trace_id": "xxx", "device_id": "bedroom_ac", "action": "AC_SET_COOL_26_AUTO"}
```

**Status Report:**
```
Topic: cores3/ir/bedroom/status
Payload: {"trace_id": "xxx", "device_id": "bedroom_ac", "action": "AC_SET_COOL_26_AUTO", "ok": true, "message": "sent"}
```

---

## 中文

### 为什么做这个项目

在完成 CoreS3 的基础实验后（记录在 [from-zero-to-cores3](https://github.com/Jonathan987321123/from-zero-to-cores3)），我想构建一个实用的家庭自动化系统。

核心问题是：**如何让 CoreS3 不只是一个单独设备，而是完整家庭自动化生态系统的一部分？**

### 系统架构

系统设计有明确的角色定义：

```
CoreS3 = 移动语音入口 + 房间红外执行节点
Home Assistant = 家庭中枢 + 自动化编排 + 设备路由
Cloud Worker = ASR/NLU 语音理解层
```

### 两条并行发展路线

#### 路线一：云端自主语音

- CoreS3 自己录音、上传云端、拿回 command，再由本地红外执行
- 适合电视、空调这类"说一句，附近设备立即执行"的场景

#### 路线二：HA MQTT 路线

- CoreS3 通过 MQTT 接入 Home Assistant
- HA 做自动化和家庭设备编排
- 适合多设备联动、状态管理、脚本自动化

### 技术挑战

#### 挑战一：唤醒词检测

嵌入式设备上的唤醒词检测很棘手。使用 ESP-SR 框架配合正确的 PSRAM 配置。

#### 挑战二：MQTT 与 HA 集成

Home Assistant 的 MQTT Discovery 协议很强大，但需要仔细处理 payload 格式。

#### 挑战三：红外控制时序

空调红外码有严格的时序要求。几微秒的差异可能意味着"开机"和"无反应"的区别。

#### 挑战四：状态持久化

空调状态需要在重启后保持，避免"忘记"温度设置。

### 我学到了什么

- **明确的角色定义是必需的**：CoreS3 不应该试图做所有事情
- **HA 是大脑，CoreS3 是执行器**：这种关注点分离使系统易于维护
- **红外码是执行层资产**：它们可以在不同的语音/云提供商之间重用
- **NVS 是你的朋友**：状态持久化防止重启后的意外行为

### 当前成果

| 设备 | 命令 | 状态 |
|------|------|------|
| 卧室空调 | 制冷 | ✅ |
| 卧室空调 | 制热 | ✅ |
| 卧室空调 | 风速 | ✅ |
| 卧室空调 | 睡眠模式 | ✅ |
| 卧室电视 | POWER/SOURCE/MENU/BACK/OK/MUTE/VOL | ✅ |
| 离家模式 | 自动关闭设备 | 🔄 开发中 |

### 当前 MQTT Topic

**语音意图上报：**
```
Topic: cores3/voice/intent
Payload: {"trace_id": "xxx", "source": "cores3_mobile", "room": "bedroom", "device_id": "bedroom_ac", "domain": "ac", "action": "AC_SET_COOL_26_AUTO", "text": "卧室空调制冷26度"}
```

**HA 下发执行命令：**
```
Topic: cores3/ir/bedroom/execute
Payload: {"trace_id": "xxx", "device_id": "bedroom_ac", "action": "AC_SET_COOL_26_AUTO"}
```

**状态回报：**
```
Topic: cores3/ir/bedroom/status
Payload: {"trace_id": "xxx", "device_id": "bedroom_ac", "action": "AC_SET_COOL_26_AUTO", "ok": true, "message": "sent"}
```
