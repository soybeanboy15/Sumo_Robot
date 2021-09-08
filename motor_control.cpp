#include "motor_control.h"

#include "config.h"

//Initialise States
MotorStates motorState = PAUSE;
MotorStates prevMotorState;


//Create motor objects
MotorControl motors(MOTORSPEED, TURNSPEED, 0, ACCELERATION);

//Actuates motors and makes soundsbased on states.
void sendMotorControl() {
  switch(motorState) {
    case PAUSE:
      motors.pause();
      prevMotorState = PAUSE;
      break;
    case FORWARD:
      motors.accelerate();
      motors.forward();
      prevMotorState = FORWARD;
      break;
    case REVERSE:
      motors.accelerate();
      motors.reverse();
      prevMotorState = REVERSE;
      break;
    case LEFT:
      motors.accelerate();
      motors.left();
      prevMotorState = LEFT;
      break;
    case RIGHT:
      motors.accelerate();
      motors.right();
      prevMotorState = RIGHT;
      break;
    case FORWARD_LEFT:
      motors.accelerate();
      motors.forwardLeft();
      prevMotorState = FORWARD_LEFT;
      break;
    case FORWARD_RIGHT:
      motors.accelerate();
      motors.forwardRight();
      prevMotorState = FORWARD_RIGHT;
      break;
    case HARD_REVERSE:
      motors.hardReverse();
      prevMotorState = HARD_REVERSE;
      break;
    case DEACCELERATION_PAUSE:
        motors.brake();
        switch(prevMotorState) {
          case FORWARD: 
            motors.forward();
            break;
          case REVERSE:
            motors.reverse();
            break;
          case RIGHT:
            motors.right();
            break;
          case LEFT:
            motors.left();
            break;
          case FORWARD_RIGHT:
            motors.forwardRight();
            break;
          case FORWARD_LEFT:
            motors.forwardLeft();
            break;
          case HARD_REVERSE:
            motors.pause();
          case PAUSE:
            motors.pause();
        }
        break;       
  }
}

MotorControl::MotorControl(uint16_t ms, uint16_t ts, uint16_t cs, uint8_t a) 
{
  motorSpeed = ms;
  turningSpeed = ts;
  currentSpeed = cs;
  acceleration = a;
}

void MotorControl::forward() 
{
  setSpeeds(currentSpeed, currentSpeed);
}

void MotorControl::reverse() 
{
  setSpeeds(-currentSpeed, -currentSpeed);
}

void MotorControl::left() 
{
  if(currentSpeed > turningSpeed) {
    currentSpeed = turningSpeed;
  }
  setSpeeds(-currentSpeed, currentSpeed);
}

void MotorControl::right() 
{
  if(currentSpeed > turningSpeed) {
    currentSpeed = turningSpeed;
  }
  setSpeeds(currentSpeed, -currentSpeed);
}

void MotorControl::forwardLeft() 
{
  setSpeeds(currentSpeed/4, currentSpeed);
}


void MotorControl::forwardRight() {
  setSpeeds(currentSpeed, currentSpeed/4);
}

void MotorControl::pause()
{
  setSpeeds(0, 0);
  currentSpeed = 0;
}

void MotorControl::brake()
{
  if(currentSpeed > 0) {
    currentSpeed -= acceleration;
  }
  else {
    currentSpeed = 0;
  }
}

void MotorControl::accelerate() {
  if(currentSpeed < 50) {
    currentSpeed = 100;
  }
  if(currentSpeed < motorSpeed) {
    currentSpeed += acceleration;
  }
}

void MotorControl::hardReverse() {
  setSpeeds(-150,-150);
  currentSpeed = 0;
}

  
