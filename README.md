
# IoT-Based Water Quality Monitoring device using ESP32

This open-source project monitors water quality using an ESP32 microcontroller and sensors like pH, EC, TDS, turbidity, and temperature. It collects real-time data and sends it to an IoT platform for monitoring, visualization, and alerting.

##  Features
- Real-time water quality monitoring
- Measures:
  - pH level
  - EC value
  - TDS
  - Turbidity (clarity)
  - Temperature
- Sends data to Blynk cloud platforms 
- Compact and low-cost design using ESP32
- Suitable for rural water testing, aquariums, and environmental studies

## 🛠 Hardware Components
- ESP32 Dev Board
- pH Sensor
- Turbidity Sensor
- Conductivity sensor
- Waterproof Temperature Sensor
- 5V Power Supply

##  Software & Platforms
- Arduino IDE
- Libraries:
  - `WiFi.h`
  - `OneWire.h` & `DallasTemperature.h`
  - `BlynkSimpleEsp32.h` % `U8g2lib.h`
- IoT Platform: [Blynk](https://blynk.cloud/dashboard/369854/global/devices/1/organization/369854/devices/1295413/dashboard)

##  How It Works
1. Sensors collect water data and send it to ESP32.
2. ESP32 processes the data and sends it via Wi-Fi to the cloud.
3. Cloud dashboard displays real-time values and trends.
4. Optional: Set thresholds to trigger alerts (e.g., if pH < 6.5 or turbidity is high).

