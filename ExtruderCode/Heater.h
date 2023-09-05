#include <Arduino.h>
#include <PID_v1.h>

#define relay1 7 //tip
#define relay2 50 //end
#define relay3 51 //middle

void TurnOnHeater();
void TurnOffHeater();
void SetUpPID();
void RunPID(int Setpoint);