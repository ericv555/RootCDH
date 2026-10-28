idf_component_register(
    SRCS "main.cpp" "sensors/DFRobot_Oxygen.cpp"
    INCLUDE_DIRS "."
    REQUIRES DFRobot_OxygenSensor espressif__arduino-esp32
)
