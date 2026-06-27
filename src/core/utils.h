#pragma once

#include <Arduino.h>

void printBanner();
bool setError(String& errorMsg, const String& msg);
String cleanLine(String line);