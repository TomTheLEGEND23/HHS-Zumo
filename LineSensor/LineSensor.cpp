#include "LineSensor.h"
#include <Arduino.h>

LineSensor::lineSensors()
{
    // Constructor
}

void LineSensor::init()
{
    lineSensors.initFiveSensors(); // Initialize the line sensors
}

bool LineSensor::isBlackLineDetected()
{
    uint16_t lineSensorValues[NUM_SENSORS];
    lineSensors.read(lineSensorValues); // Read raw sensor values

    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
        if (lineSensorValues[i] > BLACK_LINE_THRESHOLD)
        {
            return true; // Black line detected
        }
    }
    return false; // No black line detected
}

void LineSensor::readSensorValues(uint16_t *sensorValues)
{
    lineSensors.read(sensorValues); // Read raw sensor values
}

void LineSensor::printSensorValues()
{
    uint16_t lineSensorValues[NUM_SENSORS];
    lineSensors.read(lineSensorValues); // Read raw sensor values

    Serial.print("Sensor Values: ");
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
        Serial.print(lineSensorValues[i]);
        Serial.print(" ");
    }
    Serial.println();
}
