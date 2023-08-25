#include <Arduino.h>
#include <PID_v1.h>

#define relay1 7
#define relay2 50
#define relay3 51

void TurnOnHeater();
void TurnOffHeater();
void SetUpPID();
void RunPID(int Setpoint);