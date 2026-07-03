# Configuration Guide / 配置指南

## English

This project uses ESP-IDF's Kconfig system for configuration. Sensitive information like WiFi passwords and tokens are stored in Kconfig instead of hardcoded in source files.

### Configuration File

The `main/Kconfig.projbuild` file defines all configurable parameters.

### How to Configure

1. **Open menuconfig**
   ```bash
   cd cores3_wakeup_poc
   . $IDF_PATH/export.sh          # Linux/macOS
   # or
   % IDF_PATH%\export.bat          # Windows

   idf.py menuconfig
   ```

2. **Navigate to CoreS3 Wake-up PoC Configuration**

3. **Configure the following parameters:**

   | Parameter | Description | Example |
   |-----------|-------------|---------|
   | WiFi SSID | Your WiFi network name | `MyHomeWiFi` |
   | WiFi Password | Your WiFi password | `********` |
   | Device Token | Cloud service authentication | `your-token` |
   | Device ID | Unique device identifier | `cores3-bedroom` |
   | MQTT Broker URI | MQTT broker address | `mqtt://192.168.3.210:1884` |
   | MQTT Username | MQTT username (if auth enabled) | `mqtt_user` |
   | MQTT Password | MQTT password (if auth enabled) | `mqtt_pass` |
   | Cloud Voice URL | ASR/NLU service endpoint | `https://s3.example.com/v1/voice` |
   | IR TX GPIO | IR transmitter pin | `9` |

4. **Save and exit**

5. **Rebuild the project**
   ```bash
   idf.py build
   ```

---

## 中文

本项目使用 ESP-IDF 的 Kconfig 系统进行配置。敏感信息（如 WiFi 密码和 Token）存储在 Kconfig 中，而不是硬编码在源文件中。

### 配置文件

`main/Kconfig.projbuild` 文件定义了所有可配置参数。

### 如何配置

1. **打开 menuconfig**
   ```bash
   cd cores3_wakeup_poc
   . $IDF_PATH/export.sh          # Linux/macOS
   # 或
   % IDF_PATH%\export.bat          # Windows

   idf.py menuconfig
   ```

2. **导航到 CoreS3 Wake-up PoC Configuration**

3. **配置以下参数：**

   | 参数 | 说明 | 示例 |
   |------|------|------|
   | WiFi SSID | 您的 WiFi 名称 | `MyHomeWiFi` |
   | WiFi Password | 您的 WiFi 密码 | `********` |
   | Device Token | 云服务认证令牌 | `your-token` |
   | Device ID | 唯一设备标识符 | `cores3-bedroom` |
   | MQTT Broker URI | MQTT broker 地址 | `mqtt://192.168.3.210:1884` |
   | MQTT Username | MQTT 用户名（如果启用了认证） | `mqtt_user` |
   | MQTT Password | MQTT 密码（如果启用了认证） | `mqtt_pass` |
   | Cloud Voice URL | ASR/NLU 服务端点 | `https://s3.example.com/v1/voice` |
   | IR TX GPIO | 红外发射引脚 | `9` |

4. **保存并退出**

5. **重新构建项目**
   ```bash
   idf.py build
   ```

### Configuration Parameters Detail / 配置参数详解

#### WiFi Settings / WiFi 设置

```
Component config → CoreS3 Wake-up PoC Configuration → WiFi SSID
Component config → CoreS3 Wake-up PoC Configuration → WiFi Password
```

Required for device network connectivity.

#### MQTT Settings / MQTT 设置

```
Component config → CoreS3 Wake-up PoC Configuration → MQTT Broker URI
Component config → CoreS3 Wake-up PoC Configuration → MQTT Username
Component config → CoreS3 Wake-up PoC Configuration → MQTT Password
```

MQTT is used for Home Assistant integration.

#### Cloud Service / 云服务

```
Component config → CoreS3 Wake-up PoC Configuration → Cloud Voice URL
Component config → CoreS3 Wake-up PoC Configuration → Device Token
```

Cloud voice ASR/NLU service for speech recognition.

#### Device Settings / 设备设置

```
Component config → CoreS3 Wake-up PoC Configuration → Device ID
Component config → CoreS3 Wake-up PoC Configuration → IR TX GPIO
```

Unique identifier and IR transmitter pin configuration.
