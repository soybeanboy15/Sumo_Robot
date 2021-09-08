#include "obstacle_proximity.h"
#include "config.h"
#include <Zumo32U4.h>

Zumo32U4ProximitySensors proxSensors;
bool obstacleDetected = false;

void initObstacleProx() {
  proxSensors.initThreeSensors();
}

MotorDiv pollObstacle() {
  MotorDiv motordiv;
  proxSensors.read();
  uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
  uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
  uint8_t _leftValue = proxSensors.countsLeftWithLeftLeds();
  uint8_t _rightValue = proxSensors.countsRightWithRightLeds();
  bool objectSeen = leftValue >= PROX_THRESHOLD || rightValue >= PROX_THRESHOLD;
 
  if(objectSeen) {
    if(leftValue > rightValue) {
      motordiv.rightDiv = leftValue/2;
      motordiv.leftDiv = 1;
    }
    else if(rightValue > leftValue){
      motordiv.rightDiv = 1;
      motordiv.leftDiv = rightValue/2;
    }
    else {
      if(_leftValue > _rightValue) {
        motordiv.rightDiv = leftValue/2;
        motordiv.leftDiv = 1;
      }
      else {
        motordiv.rightDiv = 1;
        motordiv.leftDiv = rightValue/2;
      }
    }
  } 
  else {
    obstacleDetected = false;
    motordiv.leftDiv = 1;
    motordiv.rightDiv = 1;
  }
  return motordiv;
}
