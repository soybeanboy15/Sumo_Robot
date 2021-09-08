#include <Zumo32U4.h>

#include "config.h"
#include "edge_detection.h"
#include "obstacle_proximity.h"
#include "openmv.h"


// Initialise state
States state = _IDLE;
// Initialise struct for serial message from OpenMV
Msg msg;
MotorDiv motordiv;

Zumo32U4Motors motor;
void setup() {
  // Start serial connection with OpenMV
  Serial1.begin(115200);
  
  // Intialize edge detection
  initEdgeDetection();

  // Initialize Proximity Sensors
  initObstacleProx();
  
}

void loop() {
  // Send data to openMV
  char s[] = "<beeeep>";
  Serial1.write(s);
  
  // Receive OpenMV Serial Command
  msg = recvH7Msg();
  
  // --- State Change ---
  // Change state to obstacle avoidance mode depending on if button x was pressed on controller
  if(msg.cmd[1] == '1' && state != EDGE_DETECTED) {
    state = OBSTACLE_DETECTED;
    motordiv = pollObstacle();
    ledYellow(1);
  }
  else if(msg.cmd[1] == '0' && state != EDGE_DETECTED) {
    state = _IDLE;
    ledYellow(0);
  }
  
  // Poll edge and change state  to edge detected. (Always on)
  pollEdge();
  
  
  // Actuate Motors depending on states:
  switch(state) {
    case EDGE_DETECTED:
      motor.setSpeeds(-250, -250);
      break;
    case _IDLE: 
      motor.setSpeeds(msg.leftSpeed, msg.rightSpeed);
      break;
    case OBSTACLE_DETECTED: 
      motor.setSpeeds(msg.leftSpeed/motordiv.leftDiv, msg.rightSpeed/motordiv.rightDiv);
      break;     
  }
  Serial.print(motordiv.leftDiv);
  Serial.print("  ");
  Serial.print(motordiv.rightDiv);
  Serial.print("  ");
  Serial.print(msg.leftSpeed);
  Serial.print("  ");
  Serial.println(msg.leftSpeed);
}
