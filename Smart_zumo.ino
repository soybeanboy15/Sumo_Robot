#include <Zumo32U4.h>
#include "motor_control.h"
#include "bluetooth_serial.h"
#include "line_sensors.h"
#include "obstacle_proximity.h"
#include "config.h"
#include <Chrono.h>
#include <Wire.h>



//Chrono class keeps track of timer passed using millis() to allow for multitasking
Chrono chronoIdle;
Chrono chronoBluetooth;
Chrono chronoLineSensor;
Chrono chronoProx;
Chrono chronoMotorControl;

//Count to see how many cycles has passed
volatile uint32_t count = 0;

void setup() {
  //Setup serial for bluetooth
  Serial1.begin(57600);
  delay(500);
  Serial1.println("hello"); 
  delay(100);

  //Initialise line sensors.
  initEdgeDetection();
  //Initialise prox sensors.
  initObstacleProx();
}

void loop() {
  
  pollEdge();
  if(!edgeDetected) {
    if(!obstacleDetected) {
      pollBluetoothCommands();
    }
    if(obstacleAvoidance) {
      if(chronoProx.hasPassed(1000/PROX_SENSOR_FREQ)) {
        chronoProx.restart();
        pollObstacle();
      }
    } else {
      Serial1.println("Manual Mode");
    }
  }

  if(chronoMotorControl.hasPassed(1000/MOTOR_CONTROL_FREQ)) {
    chronoMotorControl.restart();
    sendMotorControl();
  }

}
