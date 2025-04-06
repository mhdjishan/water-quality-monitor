#define BLYNK_TEMPLATE_ID "TMPL3qaz2fhjO"   
#define BLYNK_TEMPLATE_NAME "IoT water quality meter"  
#define BLYNK_AUTH_TOKEN "ZrEr1NvhAOqHIA5FEyOKq7seNm7vPWIb"  

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <U8g2lib.h>

// WiFi credentials
const char* ssid = "Jishan";          
const char* password = "91929394";    

// Pin Definitions
const int TDS_PIN = 33;  
const int ONE_WIRE_BUS = 15;  
const int turbidityPin = 35;
const int phSensorPin = 36;

// Calibration parameters
float calibrationOffset = 0.0;  

// Constants
const float VREF = 3.3;  
const int ADC_RES = 4095;  
const float CALIBRATION = 1.0;  

// Create OneWire instance for DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Create OLED display object for SH1106 (I2C Address: 0x3C)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Variables
float voltage, tdsValue;
float temperature = 25;  
int sensorValue2 = 0;
float voltage2 = 0;
float turbidity = 0;
int sensorValue3 = 0;
float voltage3 = 0;
float pHValue = 0;

void setup() {
  Serial.begin(115200);  

  // Start temperature sensor
  sensors.begin();

  // Initialize the OLED display
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "Water Quality");
  u8g2.drawStr(0, 22, "Monitoring...");
  u8g2.sendBuffer();
  delay(2000);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Read temperature from DS18B20 sensor
  sensors.requestTemperatures();  
  temperature = sensors.getTempCByIndex(0);  
  
  if (temperature == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
  } else {
    Serial.print("Temperature: ");
    Serial.println(temperature);
  }

  // Read TDS sensor
  int sensorValue1 = analogRead(TDS_PIN);
  voltage = sensorValue1 * (VREF / ADC_RES);  
  tdsValue = CALIBRATION * (133.42 * pow(voltage, 3) - 255.86 * pow(voltage, 2) + 857.39 * voltage);
  tdsValue = tdsValue / (1.0 + 0.02 * (temperature - 25));
  if (tdsValue < 0) tdsValue = 0;  

  // Read Turbidity
  sensorValue2 = analogRead(turbidityPin);
  voltage2 = sensorValue2 * (VREF / ADC_RES);  
  if (voltage2 < 2.5) {
    turbidity = 3000.0 - (1000.0 * voltage2);
  } else {
    turbidity = (sensorValue2 - 3000) / 1000;
  }

  // Read pH Sensor
  sensorValue3 = analogRead(phSensorPin);
  voltage3 = sensorValue3 * (VREF / ADC_RES);  
  pHValue = 3.5 * voltage3 + calibrationOffset;  

  // Print results to Serial Monitor
  Serial.print("TDS Value: "); Serial.print(tdsValue, 2); Serial.println(" ppm");
  Serial.print("Turbidity: "); Serial.print(turbidity, 2); Serial.println(" NTU");
  Serial.print("pH Value: "); Serial.print(pHValue, 2); Serial.println();
  Serial.print(sensorValue1, 2);

  // Send data to Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, tdsValue);
  Blynk.virtualWrite(V2, turbidity);
  Blynk.virtualWrite(V3, pHValue);

  // Display the values on SH1106 OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 10); u8g2.print("T: "); u8g2.print(temperature, 1); u8g2.print(" C");
  u8g2.setCursor(0, 25); u8g2.print("TDS: "); u8g2.print(tdsValue, 0); u8g2.print(" ppm");
  u8g2.setCursor(0, 40); u8g2.print("Turb: "); u8g2.print(turbidity, 1); u8g2.print(" NTU");
  u8g2.setCursor(0, 55); u8g2.print("pH: "); u8g2.print(pHValue, 1);
  u8g2.sendBuffer();

  delay(2000);
}
