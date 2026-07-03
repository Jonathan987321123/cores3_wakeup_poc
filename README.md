# CoreS3 家庭智能中心 / CoreS3 Home Smart Center

[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-5.5.4-blue)](https://github.com/espressif/esp-idf)
[![Platform](https://img.shields.io/badge/Platform-ESP32--S3-orange)](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_cn.pdf)
[![Hardware](https://img.shields.io/badge/Hardware-M5Stack%20CoreS3-green)](https://docs.m5stack.com/en/core/CoreS3)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Supported-blueviolet)](https://www.home-assistant.io/)
[![License](https://img.shields.io/badge/License-MIT-lightgrey)](LICENSE)

---

## English

This is a **CoreS3 Home Smart Center** project that combines voice control, Home Assistant integration, and IR device control into one cohesive system.

### Core Architecture / 核心架构

```
人说话
  -> CoreS3 唤醒/录音
  -> Cloud ASR/NLU (Cloudflare Worker)
  -> CoreS3 发布 MQTT intent 给 HA
  -> HA 判断房间、设备、场景
  -> HA 发布 MQTT execute 给 CoreS3
  -> CoreS3 只负责本地 IR 执行
  -> CoreS3 回报 status
```

### Key Features / 主要功能

| Feature | Description |
|---------|-------------|
| **Wake-up Word** | ESP-SR based wake word detection ("Hey CoreS3") |
| **Cloud Voice** | Cloud-based ASR/NLU for natural language commands |
| **HA Integration** | Home Assistant MQTT Discovery for automation |
| **AC Control** | Air conditioner IR control with temperature presets |
| **TV Control** | TV IR control (Sanyo and other brands) |
| **Away Mode** | Presence detection and auto device management |
| **Mobile Detection** | WiFi/Bluetooth based presence tracking |

### System Design / 系统设计

#### Device Role Definition / 设备角色定义

```
CoreS3 = 移动语音入口 + 房间红外执行节点
Home Assistant = 家庭中枢 + 自动化编排 + 房间/设备路由
Cloud Worker = ASR/NLU 语音理解层
```

#### Two Development Lines / 两条发展路线

1. **Cloud Voice Line / 云端自主语音路线**
   - CoreS3 自己录音、上传云端、拿回 command，再由本地红外执行
   - 适合电视、空调这类"说一句，附近设备立即执行"的场景

2. **HA MQTT Line / HA MQTT 路线**
   - CoreS3 通过 MQTT 接入 Home Assistant
   - HA 做自动化和家庭设备编排
   - 适合多设备联动、状态管理、脚本自动化

### Hardware Requirements / 硬件需求

| Component | Specification |
|-----------|---------------|
| Main Board | M5Stack CoreS3 |
| MCU | ESP32-S3 (Xtensa dual-core) |
| Display | 320x240 ILI9342C TFT |
| Audio | Built-in microphone array + speaker |
| IR | GPIO 9 infrared transmitter |
| Storage | 16MB Flash + 8MB PSRAM |

### Tech Stack / 技术栈

| Layer | Technology |
|-------|------------|
| Framework | ESP-IDF 5.x |
| Voice Recognition | ESP-SR + Cloud ASR/NLU |
| MQTT | esp-mqtt |
| Cloud Services | Cloudflare Worker + MiniMax API |
| Home Automation | Home Assistant MQTT Discovery |
| Display | M5Stack M5GFX / LovyanGFX |

### MQTT Topic Structure / MQTT Topic 结构

```
cores3/<device_id>/
├── voice/intent          # Voice intent report to HA
├── ir/<room>/execute    # IR execution command from HA
├── ir/<room>/status     # IR execution result report
├── away/activate        # Away mode activation
└── away/deactivate      # Away mode deactivation
```

---

## 中文

这是一个 **CoreS3 家庭智能中心** 项目，将语音控制、Home Assistant 集成和红外设备控制整合为一个完整的系统。

### 项目定位

CoreS3 现在不是一个单一项目，而是两条可并行发展的路线：

#### 路线一：云端自主语音

- CoreS3 自己录音、上传云端、拿回 command，再由本地红外执行
- 适合电视、空调这类"说一句，附近设备立即执行"的场景

#### 路线二：Home Assistant MQTT

- CoreS3 通过 MQTT 接入 Home Assistant
- HA 做自动化和家庭设备编排
- 适合多设备联动、状态管理、脚本自动化和后续家庭中心扩展

### 当前已确认能力

| 设备 | 命令 | 状态 |
|------|------|------|
| 卧室空调 | 制冷已通 | ✅ |
| 卧室空调 | 制热已通 | ✅ |
| 卧室空调 | 风速已通 | ✅ |
| 卧室空调 | 睡眠已通 | ✅ |
| 卧室电视 | POWER/SOURCE/MENU/BACK/OK/MUTE/VOL | ✅ |
| 离家模式 | 自动关闭设备 | 🔄 开发中 |

### 项目结构

```
cores3_wakeup_poc/
├── main/
│   ├── ac/                          # 空调控制模块
│   │   ├── ac_control_minimal.cpp   # 最小化空调控制接口
│   │   ├── ac_ha_router_client.cpp  # HA 空调命令路由
│   │   ├── ac_intent_ingress.cpp   # 空调语音意图解析
│   │   ├── ac_preset_registry.cpp   # 温度预设注册表
│   │   ├── ac_state_memory.cpp      # 状态持久化 (NVS)
│   │   └── ac_temperature_registry.cpp # 温度预设定义
│   ├── tv/                          # 电视控制模块
│   │   └── tv_ha_router_client.cpp  # HA 电视命令路由
│   ├── mqtt_bridge.cpp/h            # MQTT 桥接器
│   ├── display_status.cpp/h          # CoreS3 屏幕状态显示
│   └── hello_world_main.cpp         # 主程序入口
├── docs/
│   ├── Configuration-Guide.md       # 配置指南
│   └── HA-Configuration-Guide.md    # HA 配置指南
└── records/                          # 阶段记录
    ├── README.md
    ├── 02-mqtt-bridge.md
    ├── 03-ac-control.md
    └── ...
```

### 模块职责分工

```
hello_world_main     → 负责调度
ac_intent_ingress    → 负责识别语音意图
ac_ha_router_client  → 负责上送 HA
mqtt_bridge          → MQTT 通信
ac_temperature_registry / ac_temp_assets → 负责资产化
ac_control_minimal   → 负责本地 IR 执行
ac_state_memory      → 负责状态记忆
display_status       → 负责屏显
```

### Home Assistant 配置

HA 访问地址：`http://192.168.3.210:8123`
MQTT Broker：`mqtt://192.168.3.210:1884`

详见 [docs/HA-Configuration-Guide.md](docs/HA-Configuration-Guide.md)

### 构建与配置

```bash
# 克隆项目
git clone https://github.com/Jonathan987321123/cores3_wakeup_poc.git
cd cores3_wakeup_poc

# 设置 ESP-IDF 环境
. $IDF_PATH/export.sh    # Linux/macOS
# 或
% IDF_PATH%\export.bat    # Windows

# 配置项目
idf.py set-target esp32s3
idf.py menuconfig

# 构建
idf.py build

# 烧录
idf.py -p /dev/ttyUSB0 flash monitor
```

### 配置说明

运行 `idf.py menuconfig` 后，配置以下选项：

```
Component config → CoreS3 Wake-up PoC Configuration
├── WiFi SSID                 # 您的 WiFi 名称
├── WiFi Password             # 您的 WiFi 密码
├── Device Token              # 云服务认证令牌
├── Device ID                # 唯一设备标识符
├── MQTT Broker URI          # MQTT broker 地址
├── Cloud Voice URL           # ASR/NLU 服务端点
└── IR TX GPIO               # 红外发射引脚
```

详见 [docs/Configuration-Guide.md](docs/Configuration-Guide.md)

---

## 相关仓库

| 仓库 | 说明 |
|------|------|
| [from-zero-to-cores3](https://github.com/Jonathan987321123/from-zero-to-cores3) | 学习历程文档 / Learning journey |
| [cores3-wakeup-poc](https://github.com/Jonathan987321123/cores3_wakeup_poc) | 当前项目 / Current project |

## 开发路线

本项目遵循 [from-zero-to-cores3](https://github.com/Jonathan987321123/from-zero-to-cores3) 中记录的开发路径：

1. UIFlow2 基础 → [Stage 01](https://github.com/Jonathan987321123/from-zero-to-cores3/blob/main/records/01-uiflow2-foundation.md)
2. Arduino 语音遥控 → [Stage 02](https://github.com/Jonathan987321123/from-zero-to-cores3/blob/main/records/02-arduino-voice-remote.md)
3. ESP-IDF 屏幕麦克风 VAD → [Stage 05](https://github.com/Jonathan987321123/from-zero-to-cores3/blob/main/records/05-esp-idf-screen-mic-vad.md)
4. 唤醒词 PoC → **当前项目**

## 项目状态

This project is under active development.
本项目正在积极开发中。

## 致谢

- [ESP-IDF](https://github.com/espressif/esp-idf) - Espressif IoT Development Framework
- [ESP-SR](https://github.com/espressif/esp-sr) - Speech Recognition Framework
- [M5Stack](https://github.com/m5stack/M5Stack) - Hardware abstraction library
- [Home Assistant](https://www.home-assistant.io/) - Open source home automation
- [xiaozhi-esp32](https://github.com/kerubitoo/xiaozhi-esp32) - Voice assistant reference

## 许可证

MIT License - See [LICENSE](LICENSE) file for details.
