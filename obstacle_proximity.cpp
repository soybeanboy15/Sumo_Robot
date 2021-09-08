#include "obstacle_proximity.h"
#include "motor_control.h"
#include "line_sensors.h"
#include "config.h"

Zumo32U4ProximitySensors proxSensors;
const char obstacle[] PROGMEM = "! O5 V9 C8 D8";

bool obstacleDetected = false;

void initObstacleProx() {
  proxSensors.initThreeSensors();
}

void pollObstacle() {

  proxSensors.read();
  uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
  uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
  uint8_t _leftValue = proxSensors.countsLeftWithLeftLeds();
  uint8_t _rightValue = proxSensors.countsRightWithRightLeds();
  bool objectSeen = leftValue >= PROX_THRESHOLD || rightValue >= PROX_THRESHOLD;
  Serial1.print("Obstacle Detection Mode: ");
  Serial1.print("    ");
  Serial1.print("L - ");
  Serial1.print(leftValue);
  Serial1.print("               ");
  Serial1.print("R - ");
  Serial1.println(rightValue);
  
  if(objectSeen && motorState == FORWARD) {
    obstacleDetected = true;
    if(buzzerPlayed == false) {
      buzzer.playFromProgramSpace(obstacle);
      buzzerPlayed = true;
    }
    if(leftValue > rightValue) {
      motorState = FORWARD_RIGHT;
    }
    else if(rightValue > leftValue){
      motorState = FORWARD_LEFT;
    }
    else {
      if(_leftValue > _rightValue) {
        motorState = FORWARD_RIGHT;
      }
      else {
        motorState = FORWARD_LEFT;
      }
    }
  } 
  else {
    buzzerPlayed = false;
    obstacleDetected = false;
  }
}
