#include "bluetooth_serial.h"
#include "motor_control.h"
#include "config.h"
uint32_t prevTime1 = 0;

bool obstacleAvoidance = false;

void pollBluetoothCommands() {
  int serialData;
  //Read bluetooth serial commands
  if(Serial1.available()){
    prevTime1 = millis();
    serialData = Serial1.read();
    interpretCommand(serialData);
    ledYellow(1);

  } 
  //Need to set motorstate to pause, only if last serial data was sent 80ms ago.
  if(millis() - prevTime1 > 80) {
    motorState = DEACCELERATION_PAUSE;
    ledYellow(0);
  }
}


void interpretCommand(int cmd) {
  switch(cmd) {
    case 'w':
      motorState = FORWARD;
      break;
    case 's':
      motorState = REVERSE;
      break;
    case 'a':
      motorState = LEFT;
      break;
    case 'd': 
      motorState = RIGHT;
      break;
    case 'x':
      motorState = PAUSE;
      break;
    case 'z':
      motorState = FORWARD_LEFT;
      break;
    case 'c':
      motorState = FORWARD_RIGHT;
      break;

    case 'o':
      obstacleAvoidance = true;
      break;
    case 'm':
      obstacleAvoidance = false;
      break;
    case 'v':
      motorState = DEACCELERATION_PAUSE;
      break;
  }
}
