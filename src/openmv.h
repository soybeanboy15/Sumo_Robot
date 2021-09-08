#ifndef OPENMV_H
#define OPENMV_H

// Message received as string packaged into struct
struct Msg {
  char cmd[3]; // Two characters for additional commands, third one used to blink led to show connection
  float leftSpeed; // left and right motor speeds
  float rightSpeed;
};

typedef struct Msg Msg;

// Function to receive serial command and return Msg struct
Msg recvH7Msg();

// Function to receive string with start and end markers
void recvWithStartEndMarkers();


#endif
