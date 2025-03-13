#include "RelayController.h"
#include <Arduino.h>

RelayController::RelayController(short port, bool activeLow)
    : portNumber(port), activeLow(activeLow) {
    pinMode(portNumber, OUTPUT);
    this->SwitchOff();
}

void RelayController::SwitchOn(short duration) {
    // Code to open the valve
    if (activeLow) {
        digitalWrite(portNumber, LOW);
    } else {
        digitalWrite(portNumber, HIGH);
    }
    if (duration > 0) {
        // Code to handle timed opening
    }
}

void RelayController::SwitchOff() {
    digitalWrite(portNumber, !activeLow);
}
