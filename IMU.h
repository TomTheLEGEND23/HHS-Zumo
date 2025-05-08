#include <Zumo32U4.h>
#include <Wire.h>

class IMU
{
public:
    IMU(float, float);
    float accelX();
    float accelY();
    float accelZ();
    float gyroX();
    float gyroY();
    float gyroZ();
    float compassHeading();
    bool fetchStatus();
    float pitch();
    float roll();

private:
    bool status;
    float accelBiasX;
    float accelBiasY;
    float accelBiasZ;
    float gyroBiasX;
    float gyroBiasY;
    float gyroBiasZ;
    float magCalX;
    float magCalY;
    
};