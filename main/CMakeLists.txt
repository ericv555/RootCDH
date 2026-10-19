idf_component_register(
    SRCS "main.cpp" "Sensair_Sunrise/sensair_sunrise.cpp" 
    INCLUDE_DIRS "."
    REQUIRES Sensair_Sunrise espressif__arduino-esp32
)