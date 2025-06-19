#include "ProximitySensors.h"

ProximitySensors::ProximitySensors() {
  zumoProxSensors.initFrontSensor();
}

bool ProximitySensors::objectDetectedFront() {
  zumoProxSensors.read();
  return (zumoProxSensors.countsFrontWithLeftLeds() > detectionThreshold) || (zumoProxSensors.countsFrontWithRightLeds() > detectionThreshold);
}

void ProximitySensors::printReadings() {
  zumoProxSensors.read();

  Serial.print("\tF: ");
  Serial.print(zumoProxSensors.countsFrontWithLeftLeds());
  Serial.print("/");
  Serial.print(zumoProxSensors.countsFrontWithRightLeds());

  Serial.print(" F:");
  Serial.print(objectDetectedFront() ? "Y" : "N");
  Serial.println("");
}
