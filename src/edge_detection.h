#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H
#include <Zumo32U4.h>

// Function to set up line sensors
void initEdgeDetection();

// Function to check if edge is detected and change state
void pollEdge();

// Expand Zumo Line Sensors Class

class LineSensors:public Zumo32U4LineSensors {
    public:
      LineSensors(uint16_t threshold);
      uint16_t lineSensorValues[3] = {0, 0, 0};
      uint16_t lineThreshold;
      bool edgeLeft();
      bool edgeRight();
      bool edgeCentre();
};

#endif
