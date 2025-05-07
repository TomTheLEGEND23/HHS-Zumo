class Accelerometer
{
public:
    Accelerometer();
    ~Accelerometer();
    float Accelx();
    float Accely();
    float Accelz();

private:
    int status();
};