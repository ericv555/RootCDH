idf_component_register(
    SRCS "main.cpp" "sensors/sensair_sunrise.cpp" 
    INCLUDE_DIRS "."
    REQUIRES Sensair_Sunrise espressif__arduino-esp32
)