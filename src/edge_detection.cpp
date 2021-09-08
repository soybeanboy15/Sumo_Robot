#include "edge_detection.h"
#include "config.h"

// Intialize line sensor object
LineSensors lineSensors(EDGE_THRESHOLD);

uint32_t edge_time = 0;
bool edgeDetected = 0;

// Function to intialize line sensors.
void initEdgeDetection() {
  lineSensors.initThreeSensors();
}

// Poll edge and change motorStates
void pollEdge() {
  lineSensors.read(lineSensors.lineSensorValues);
  if(lineSensors.edgeLeft()|| lineSensors.edgeRight() || lineSensors.edgeCentre()) {
    state = EDGE_DETECTED;
    edgeDetected = true;
    edge_time = millis();
  } else {
    // Lock state for 400ms if edge is detected
    if(millis() - edge_time > 400) {
      if(edgeDetected) {
        state = _IDLE;
        edgeDetected = false;
      }
    }
  } 
}

// Class constructor and method definitions
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
