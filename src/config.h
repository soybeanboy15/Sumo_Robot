#ifndef CONFIG_H
#define CONFIG_H

// Motor Properties
#define MOTORSPEED 300
#define TURNSPEED 120
#define ACCELERATION 2 

// EDGE DETECTION THRESHOLD
#define EDGE_THRESHOLD 1100

// PROXIMITY SENSOR THRESHOLD
#define PROX_THRESHOLD 5




//
enum States {
  EDGE_DETECTED = 0,
  OBSTACLE_DETECTED = 1,
  COLLISION_DETECTED = 2,
  _IDLE = 3
};

//Initiate the motorstate and previous motorState
extern States state;
#endif
