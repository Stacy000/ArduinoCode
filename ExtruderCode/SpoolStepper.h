#include <AccelStepper.h>
#include <MultiStepper.h>

// Define pin for DC motor
#define DC1 36
#define DC2 38
#define DC_EnB 6

// Define pin for limit switch
#define LS 19 //Interrupt pin

void Homing();
void Prepare();
void SpoolingFoward();
void SpoolingBackward();
void RunDCMotor();
void stopMotor();

