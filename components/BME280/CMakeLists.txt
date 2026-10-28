idf_component_register(
    SRCS
        "Adafruit_BME280.cpp"
        "Adafruit_I2CDevice.cpp"
        "Adafruit_SPIDevice.cpp"
        "Adafruit_Sensor.cpp"
    INCLUDE_DIRS "."
    REQUIRES espressif__arduino-esp32
)
