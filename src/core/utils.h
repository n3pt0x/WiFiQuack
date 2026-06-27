#pragma once

#include <Arduino.h>

void printBanner();
bool setError(String& errorMsg, const String& msg);
bool returnError(String& errorMsg);
String cleanLine(String line);