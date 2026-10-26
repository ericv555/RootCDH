/***************************************************************************
  BME280 temperature / humidity / pressure -> Serial Monitor (ESP32, I2C)

  Wiring (ESP32 default I2C pins):
    BME280 VIN/VCC -> 3V3
    BME280 GND     -> GND
    BME280 SDA     -> GPIO 21
    BME280 SCL     -> GPIO 22

  Serial Monitor: 115200 baud
 ***************************************************************************/

 #include <Wire.h>
 #include <Adafruit_Sensor.h>
 #include <Adafruit_BME280.h>
 
 // ---- I2C pins (ESP32 defaults; change if your board uses others) ----
 #define I2C_SDA 21
 #define I2C_SCL 22
 
 #define SEALEVELPRESSURE_HPA (1013.25)
 
 Adafruit_BME280 bme;  // I2C
 
 const unsigned long READ_INTERVAL_MS = 1000;
 unsigned long lastRead = 0;
 
 void setup() {
   Serial.begin(115200);
   delay(1000);  // give the serial monitor time to connect
   Serial.println(F("\nBME280 ESP32 test"));
 
   Wire.begin(I2C_SDA, I2C_SCL);
 
   // BME280 address is 0x76 or 0x77 depending on the breakout; try both
   bool found = bme.begin(0x76, &Wire);
   if (!found) {
     found = bme.begin(0x77, &Wire);
   }
 
   if (!found) {
     Serial.println(F("Could not find a valid BME280 sensor, check wiring, address, sensor ID!"));
     Serial.print(F("SensorID was: 0x")); Serial.println(bme.sensorID(), 16);
     Serial.println(F("   ID of 0xFF probably means a bad address, a BMP 180 or BMP 085"));
     Serial.println(F("   ID of 0x56-0x58 represents a BMP 280,"));
     Serial.println(F("   ID of 0x60 represents a BME 280."));
     Serial.println(F("   ID of 0x61 represents a BME 680."));
     while (true) delay(10);
   }
 
   Serial.println(F("-- BME280 found, starting readings --"));
   Serial.println();
 }
 
 void loop() {
   // non-blocking timing instead of delay()
   if (millis() - lastRead >= READ_INTERVAL_MS) {
     lastRead = millis();
     printValues();
   }
 }
 
 void printValues() {
   Serial.print(F("Temperature = "));
   Serial.print(bme.readTemperature());
   Serial.println(F(" °C"));
 
   Serial.print(F("Pressure = "));
   Serial.print(bme.readPressure() / 100.0F);
   Serial.println(F(" hPa"));
 
   Serial.print(F("Approx. Altitude = "));
   Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
   Serial.println(F(" m"));
 
   Serial.print(F("Humidity = "));
   Serial.print(bme.readHumidity());
   Serial.println(F(" %"));
 
   Serial.println();
 }
 