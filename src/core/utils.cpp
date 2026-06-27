#include "core/utils.h"

void printBanner() {
  Serial.println(R"EOD(
===================================================
__        ___ _____ _  ___                   _    
\ \      / (_)  ___(_)/ _ \ _   _  __ _  ___| | __
 \ \ /\ / /| | |_  | | | | | | | |/ _` |/ __| |/ /
  \ V  V / | |  _| | | |_| | |_| | (_| | (__|   < 
   \_/\_/  |_|_|   |_|\__\_\\__,_|\__,_|\___|_|\_\

=====================================================
    )EOD");
}

bool setError(String& errorMsg, const String& msg) {
  errorMsg = msg;
  return false;
}

bool returnError(String& errorMsg) {
  Serial.println(errorMsg);
  return false;
}

String cleanLine(String line) {
  line.replace("\r", "");
  line.replace("\n", "");
  line.trim();
  return line;
}