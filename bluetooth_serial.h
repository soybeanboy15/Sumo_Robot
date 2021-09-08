#ifndef BLUETOOTH_SERIAL_H
#define BLUETOOTH_SERIAL_H

extern bool obstacleAvoidance;
//Function to update motorstates from bluetooth commands.
void pollBluetoothCommands();

//Function to intepret commands.
void interpretCommand(int cmd);

#endif
