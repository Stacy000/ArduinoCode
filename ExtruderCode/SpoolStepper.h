#include <AccelStepper.h>
#include <MultiStepper.h>

// Define pin for limit switch
#define LS 19 //Interrupt pin

void Homing();
void Prepare();
void SpoolingFoward();
void SpoolingBackward();
void stopMotor();
