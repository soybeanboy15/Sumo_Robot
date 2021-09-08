#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include <Zumo32U4.h>

//Function to send motor commands depending on the MotorStates
void sendMotorControl();

enum MotorStates {
  PAUSE = 0,
  FORWARD = 1,
  REVERSE = 2,
  LEFT = 3,
  RIGHT = 4,
  FORWARD_LEFT = 5,
  FORWARD_RIGHT = 6,
  HARD_REVERSE = 7,
  DEACCELERATION_PAUSE = 8
};

//Initiate the motorstate and previous motorState
extern MotorStates motorState;
extern MotorStates prevMotorState;


//Expand on Zumo Motor Class
class MotorControl:public Zumo32U4Motors {
    public:
      uint16_t motorSpeed;
      uint16_t turningSpeed;
      uint16_t currentSpeed;
      uint8_t acceleration;

      MotorControl(uint16_t ms, uint16_t ts, uint16_t cs, uint8_t a); //Constructor
      void forward(); //Just set speed of both motors to currentSpeed and -currentSpeed depending on direction.
      void reverse(); 
      void left();
      void right();
      void forwardLeft();
      void forwardRight();
      void pause(); //Set speeds of motors to 0.
      void brake(); //Deaccelerates the currentspeed by the acceleration, have to use in loop.
      void accelerate(); //Same as brake but accelerates.
      void hardReverse();

};

#endif
