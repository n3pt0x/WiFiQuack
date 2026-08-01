#include <Arduino.h>
#include "utils.h"
#include "debug.h"

void printBanner() {
  debugln(F(R"EOD(
===================================================
__        ___ _____ _  ___                   _    
\ \      / (_)  ___(_)/ _ \ _   _  __ _  ___| | __
 \ \ /\ / /| | |_  | | | | | | | |/ _` |/ __| |/ /
  \ V  V / | |  _| | | |_| | |_| | (_| | (__|   < 
   \_/\_/  |_|_|   |_|\__\_\\__,_|\__,_|\___|_|\_\

=====================================================
    )EOD"));
}

bool setError(String& errorMsg, const String& msg) {
  errorMsg = msg;
  return false;
}

bool returnError(String& errorMsg) {
  debugln(errorMsg);
  return false;
}

String cleanLine(String line) {
  line.replace("\r", "");
  line.replace("\n", "");
  line.trim();
  return line;
}

std::vector<String> splitParams(const String &line) {
    std::vector<String> params;
    int start = line.indexOf(' ') + 1;
    int pos = start;

    while (pos < line.length()) {
        while (pos < line.length() && line[pos] == ' ') pos++;
        if (pos >= line.length()) break;

        int end = line.indexOf(' ', pos);
        if (end == -1) end = line.length();

        params.push_back(line.substring(pos, end));
        pos = end + 1;
    }

    return params;
}