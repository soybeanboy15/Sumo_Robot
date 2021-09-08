#include "openmv.h"
#include <Arduino.h>
#include <Zumo32U4.h>
// Char array to hold received serial data
const byte numChars = 24;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
boolean newData = false;

Msg recvH7Msg() {
  Msg msg;
  char cmd[3] = {0};
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() used in parseData() replaces the commas with \0
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(cmd, strtokIndx);// copy it to cmd
    memcpy(msg.cmd, cmd, sizeof(msg.cmd));
    
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    msg.leftSpeed = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    msg.rightSpeed = atoi(strtokIndx);     // convert this part to a float
    
    //Show communication is working
    if(msg.cmd[2] == '1') {
      ledGreen(1);
    } 
    else if(msg.cmd[2] == '0') {
      ledGreen(0);
    }
    newData = false;
    return msg;
  }
}

// Function to receive string send by openMV with start and end markers defined as < and >.
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}
