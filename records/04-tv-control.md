# Stage 04 - TV Control Integration / 阶段 04 - 电视控制集成

## English

### Overview

This stage integrates TV control with voice commands. The TV control module supports basic functions and can be extended for more commands.

### Supported TV Functions

| Function | Command | HA Entity |
|----------|---------|-----------|
| Power | on/off | switch |
| Volume Up | vol+ | button |
| Volume Down | vol- | button |
| Mute | mute | switch |
| Source | source | select |
| Channel Up | ch+ | button |
| Channel Down | ch- | button |
| Navigation | up/down/left/right | dpad |
| OK | ok | button |
| Back | back | button |

### Supported TV Brands

Current implementation includes:
- Sanyo (三洋) - Primary tested brand
- More brands can be added via IR code registry

### Voice Commands

| Voice Command | Action |
|---------------|--------|
| 打开电视 / Turn on TV | Power on |
| 关闭电视 / Turn off TV | Power off |
| 调大音量 / Louder | Volume up |
| 调小音量 / Quieter | Volume down |
| 静音 / Mute | Toggle mute |
| 切换信号源 / Change source | Cycle sources |
| 下一台 / Next channel | Channel up |
| 上一台 / Previous channel | Channel down |

---

## 中文

### 概述

本阶段将电视控制与语音命令集成。电视控制模块支持基本功能，可扩展更多命令。

### 支持的电视功能

| 功能 | 命令 | HA 实体 |
|------|------|---------|
| 电源 | on/off | switch |
| 音量加 | vol+ | button |
| 音量减 | vol- | button |
| 静音 | mute | switch |
| 信号源 | source | select |
| 频道加 | ch+ | button |
| 频道减 | ch- | button |
| 导航 | 上/下/左/右 | dpad |
| 确认 | ok | button |
| 返回 | back | button |

### 支持的电视品牌

当前实现包括：
- 三洋 - 主要测试品牌
- 可通过 IR 码注册表添加更多品牌

### 语音命令

| 语音命令 | 操作 |
|----------|------|
| 打开电视 | 开机 |
| 关闭电视 | 关机 |
| 调大音量 | 音量加 |
| 调小音量 | 音量减 |
| 静音 | 切换静音 |
| 切换信号源 | 循环信号源 |
| 下一台 | 频道加 |
| 上一台 | 频道减 |
