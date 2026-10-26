idf_component_register(
    SRCS "main.cpp" "sensors/BME280.cpp"
    INCLUDE_DIRS "."
    REQUIRES BME280 espressif__arduino-esp32
)
