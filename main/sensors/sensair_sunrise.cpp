#include "sensair_sunrise.hpp"
#include "senseair_sunrise.h"
#include "esp_log.h"
#include <cstdint>
// Set esp logging tag
static constexpr char TAG[] = "sunrise";
// Set Measurement mode
constexpr MeasurementMode MEASUREMENT_MODE     = MeasurementMode::CONTINUOUS;
constexpr uint16_t        MEASUREMENT_PERIOD_S = 16u;
constexpr uint16_t        MEASUREMENT_SAMPLES  = 8u;
/* Hardware configuration */
/* Sensor communication address, use 0xFE if address is unknown and only one sensor on the bus */
constexpr uint8_t SUNRISE_ADDR = SUNRISE_DEFAULT_COMM_ADDRESS;
/* Pin to enable/disable sensor */
constexpr int SUNRISE_EN_PIN = 5;
/* Pin to select communication protocol (COMSEL): LOW = I2C, HIGH = Modbus */
constexpr int SUNRISE_COMSEL_PIN = 4;
/* Pin to read nRDY signal state */
constexpr int SUNRISE_NRDY = 7;
// Setup 
/* Global constant used to keep track of hour interval */
constexpr unsigned long HOUR_INTERVAL_MS = 3600000ul;

/* Global parameters */
static unsigned long measurementTimeStamp = 0ul;
/* Logging interval for sensor */
static unsigned long sunriseMeasurementPeriodMs = MEASUREMENT_PERIOD_S * 1000U;

static SunriseSingleModeState sunriseState;

SunriseSensor::SunriseSensor()
: sensor(Wire, SUNRISE_ADDR, SUNRISE_EN_PIN, SUNRISE_NRDY) {}
uint32_t SunriseSensor::measurementPeriodMs() const {
    return MEASUREMENT_PERIOD_S * 1000U;
}
void SunriseSensor::printErrorStatus(uint16_t errorStatus) {
    if (errorStatus == 0)
        return;
    ESP_LOGE(TAG, "  Active error flags:");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::FATAL_ERROR))
        ESP_LOGE(TAG, "  - Fatal error");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::I2C_ERROR))
        ESP_LOGE(TAG, "  - I2C error");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::ALGORITHM_ERROR))
        ESP_LOGE(TAG, "  - Algorithm error");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::CALIBRATION_ERROR))
        ESP_LOGE(TAG, "  - Calibration error");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::SELF_DIAG_ERROR))
        ESP_LOGE(TAG, "  - Self-diagnostics error");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::OUT_OF_RANGE))
        ESP_LOGE(TAG, "  - Out of range");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::MEMORY_ERROR))
        ESP_LOGE(TAG, "  - Memory error");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::NO_MEASUREMENT))
        ESP_LOGE(TAG, "  - No measurement completed");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::LOW_VOLTAGE))
        ESP_LOGE(TAG, "  - Low internal voltage");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::MEAS_TIMEOUT))
        ESP_LOGE(TAG, "  - Measurement timeout");
    if (errorStatus & static_cast<uint16_t>(ErrorStatus::ABNORMAL_SIGNAL))
        ESP_LOGE(TAG, "  - Abnormal signal level");
}
void SunriseSensor::init() {
	// Set I2C pin
    pinMode(SUNRISE_COMSEL_PIN, OUTPUT);
    digitalWrite(SUNRISE_COMSEL_PIN, LOW);
    // Begin I2C
    Wire.begin();
    Wire.setClock(SUNRISE_I2C_CLOCK_HZ);
    // Begin sensor
    sensor.begin();
    sensor.disable();
    delay(SUNRISE_STABILIZATION_MS);
    sensor.enable();
    // Error handling
    SunriseConfiguration current{};
	error = sensor.getSensorConfig(current);

	if (error != SunriseError::NO_ERROR) {
		ESP_LOGE(TAG, "Failed to read sensor configuration: %d",
				 static_cast<int>(error));
		return;
	}

	const SunriseConfiguration desired{
		MEASUREMENT_MODE,
		MEASUREMENT_PERIOD_S,
		MEASUREMENT_SAMPLES
	};

	const bool needsUpdate =
		current.measMode != desired.measMode ||
			current.measPeriod != desired.measPeriod ||
			current.measSamples != desired.measSamples;

	if (needsUpdate) {
		error = sensor.setSensorConfig(desired);
		if (error != SunriseError::NO_ERROR) {
			ESP_LOGE(TAG, "Failed to write sensor configuration: %d",
					 static_cast<int>(error));
			return;
		}

		error = sensor.resetBySCR();
		if (error != SunriseError::NO_ERROR) {
			ESP_LOGE(TAG, "Failed to reset sensor: %d",
					 static_cast<int>(error));
			return;
		}

		delay(2 * SUNRISE_STABILIZATION_MS);
	}

	ESP_LOGI(TAG, "Continuous measurement configured: period=%u s, samples=%u",
			 static_cast<unsigned>(MEASUREMENT_PERIOD_S),
			 static_cast<unsigned>(MEASUREMENT_SAMPLES));

	return;
}
void SunriseSensor::read() {
	SunriseMeasurement sunriseMeasurement = {0, 0, 0u};
	if(MEASUREMENT_MODE == MeasurementMode::SINGLE){
        error = sensor.readSingleMeasurement(sunriseMeasurement, sunriseState,
                                             MEASUREMENT_SAMPLES);
    } else {
        error = sensor.readMeasurement(sunriseMeasurement);
    }
    if (error != SunriseError::NO_ERROR) {
		printErrorStatus(sunriseMeasurement.errorStatus);
		return;
    }


}
void SunriseSensor::send_to_comms() {

}
