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

static SunriseSingleModeState sunriseState;

SunriseSensor::SunriseSensor(TwoWire &bus, uint8_t address, int enablePin, int nrdyPin, int comselPin)
: comselPin_(comselPin), sensor(bus, address, enablePin, nrdyPin){}
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
    pinMode(comselPin_, OUTPUT);
    digitalWrite(comselPin_, LOW);
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
			ESP_LOGE(TAG, "Failed to write sensor configuration: %d", static_cast<int>(error));
			return;
		}

		error = sensor.resetBySCR();
		if (error != SunriseError::NO_ERROR) {
			ESP_LOGE(TAG, "Failed to reset sensor: %d", static_cast<int>(error));
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
	ESP_LOGI(TAG, "CO2: %d ppm, Temp: %d oC",sunriseMeasurement.co2value,sunriseMeasurement.temperature);

}
void SunriseSensor::send_to_comms() {
	
}
