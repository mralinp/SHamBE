#include <Arduino.h>

#include "BleInterface/BleInterface.h"

void setup() {
    Serial.begin(115200);
    Serial.println("SHamBE the Smart GreenHouse system CORE controller...");
    
    setupBLE();   
}

void loop() {
   
}