#ifndef OBSTACLE_PROXIMITY_H
#define OBSTACLE_PROXIMITY_H
#include <Zumo32U4.h>
extern bool obstacleDetected;

struct MotorDiv {
  int leftDiv; // Variables to divide the motorspeed with to turn based on whether the obstacle is left or right.
  int rightDiv;
};

typedef struct MotorDiv MotorDiv;

void initObstacleProx();
MotorDiv pollObstacle();
#endif;
