#include "line_sensors.h"
#include "motor_control.h"
#include "config.h"

//Intialise line sensor object
LineSensors lineSensors(EDGE_THRESHOLD);
Zumo32U4Buzzer buzzer;

uint32_t prevTime2 = 0;

//Buzzer variables
const char edge[] PROGMEM = "! O3 V9 B8 F4";
bool buzzerPlayed = 0;

bool edgeDetected = 0;

//Function to init line sensors for main.
void initEdgeDetection() {
  lineSensors.initThreeSensors();
}

//Poll edge, change motor states and make sound.
void pollEdge() {
  lineSensors.read(lineSensors.lineSensorValues);
  if(lineSensors.edgeLeft()|| lineSensors.edgeRight() || lineSensors.edgeCentre()) {
    motorState = HARD_REVERSE;
    edgeDetected = true;
    prevTime2 = millis();
    //Check if sound played and play the buzzer.
    if(buzzerPlayed == false) {
      buzzer.playFromProgramSpace(edge);
      buzzerPlayed = true;
    }
  } else {
    //Locks motor state for 400ms.
    if(millis() - prevTime2 > 400) {\
      if(edgeDetected) {
        motorState = PAUSE;
        edgeDetected = false;
      }
    
      //Reset buzzer.
      buzzerPlayed = false;
    }
  }
  
}

LineSensors::LineSensors(uint16_t threshold) {
  lineThreshold = threshold;
}


bool LineSensors::edgeLeft() {
  if(lineSensorValues[0] > lineThreshold) {
    return true;
  } else {
    return false;
  }
}

bool LineSensors::edgeRight() {
  if(lineSensorValues[2] > lineThreshold) {
    return true;
  } else {
    return false;
  }
}

bool LineSensors::edgeCentre() {
  if(lineSensorValues[1] > lineThreshold) {
    return true;
  } else {
    return false;
  }
}
