#pragma once

#include <Arduino.h>

void setupBLE();
void handleBLE();
String processCommand(const char* command);