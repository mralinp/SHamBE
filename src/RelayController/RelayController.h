#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

class RelayController
{
    short portNumber; // Add port number
    bool activeLow; // Add active low/high flag
public:
    RelayController(short port, bool activeLow); // Update constructor
    void SwitchOn( short duration = -1); // Update method signature
    void SwitchOff();

};

#endif // RELAYCONTROLLER_H