#include "PMUManager.h"

#define I2C_SDA 10
#define I2C_SCL 11

static XPowersPMU pmu;

bool PMUManager::begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    if (!pmu.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
        Serial.println("Error: PMU not found.");
        return false;
    }
    Serial.println("PMU initialized.");
    return true;
}

void PMUManager::enableMicrophonePower() {
    // Enable ALDO3 (3.3V) - Required for Microphone and Touch
    pmu.setALDO3Voltage(3300);
    pmu.enableALDO3();

    // Enable ALDO1 (3.3V) - Required for RTC and Sensors
    pmu.setALDO1Voltage(3300);
    pmu.enableALDO1();
    
    Serial.println("Audio power rails enabled (ALDO1, ALDO3).");
}
