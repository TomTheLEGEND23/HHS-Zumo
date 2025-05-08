#ifndef Motor_HHS_H_
#define Motor_HHS_H_

#include <Zumo32U4.h>
#include <string.h>
class Motor_HHS {
  public:
      Motor_HHS(String, int, int);
      void init();
      void setSnelheid(int snelheid);
      int getSnelheid() const;

  private:
    String motorkant;
    int pinPWM;
    int pinDIR;
    int huidigesnelheid;
};

#endif
