#ifndef LINE_SENSORS_H
#define LINE_SENSORS_H
#include <Zumo32U4.h>

extern bool buzzerPlayed;
extern Zumo32U4Buzzer buzzer;
extern bool edgeDetected;
//Function to init line sensors for main.
void initEdgeDetection();

//Function to check if edge detected and then change motorstate to reverse.
void pollEdge();

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
