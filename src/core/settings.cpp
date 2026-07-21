#include <LittleFS.h>
#include <ArduinoJson.h>
#include "settings.h"

namespace settings {
    String wifi_ssid = "WiFiQuack";
    String wifi_passphrase = "WiFiQuack";
    keyboard_utils::Layout keyboard_layout = keyboard_utils::LAYOUT_FR;
    const char* FILENAME = "/config.json";
    const char* DEFAULT_LAYOUT_STR = "FR";
    String logBuffer;
    bool fsInitialized = false;

    String getSettingsJson() {
        String json = "{";
        json += "\"wifi_ssid\":\"" + wifi_ssid + "\",";
        json += "\"wifi_passphrase\":\"" + wifi_passphrase + "\",";
        json += "\"keyboard_layout\":\"" + layoutToString(keyboard_layout) + "\"";
        json += "}";
        return json;
    }

    bool setSettingsFromJson(const String& json) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, json);
        if (error) {
            logToBuffer("JSON parse error: ");
            logToBuffer(error.c_str());
            return false;
        }

        wifi_ssid = doc["wifi_ssid"] | wifi_ssid;
        wifi_passphrase = doc["wifi_passphrase"] | wifi_passphrase;
        String layout_str = doc["keyboard_layout"] | "FR";
        keyboard_layout = stringToLayout(layout_str);
        return true;
    }

    bool begin() {
        #ifdef PLATFORM_ESP32
            if (!LittleFS.begin()) {
                logToBuffer("LittleFS mount failed. Attempting to format...");
                
                if (!LittleFS.begin(true)) {
                    logToBuffer("LittleFS format failed. Using RAM only.");
                    fsInitialized = false;
                    return false;
                }
                logToBuffer("LittleFS formatted successfully.");
            }
        #elif defined(PLATFORM_PICO)
            if (!LittleFS.begin()) {
                logToBuffer("LittleFS mount failed. Using RAM only.");
                fsInitialized = false;
                return false;
            }
        #endif
        
        fsInitialized = true;
        logToBuffer("LittleFS mounted successfully.");

        if (!LittleFS.exists(FILENAME)) {
            logToBuffer("Config file not found. Creating default config...");
            if (!createDefaultConfig()) {
                logToBuffer("Failed to create default config.");
                return false;
            }
        }

        load();
        return true;
    }

    bool createDefaultConfig() {
        fs::File configFile = LittleFS.open(FILENAME, "w");
        if (!configFile) {
            logToBuffer("Failed to create config file.");
            return false;
        }

        String defaultConfig = getSettingsJson();
        size_t written = configFile.print(defaultConfig);
        configFile.flush();
        configFile.close();

        if (written == 0) {
            logToBuffer("Failed to write default config.");
            return false;
        }

        logToBuffer("Default config created: ");
        logToBuffer(defaultConfig);
        return true;
    }

    void load() {
        if (!fsInitialized) {
            logToBuffer("LittleFS not initialized, cannot load.");
            return;
        }

        if (!LittleFS.exists(FILENAME)) {
            logToBuffer("Config file does not exist.");
            return;
        }

        fs::File configFile = LittleFS.open(FILENAME, "r");
        if (!configFile) {
            logToBuffer("Failed to open config file for reading.");
            return;
        }

        String str = configFile.readString();
        configFile.close();

        if (str.length() == 0) {
            logToBuffer("Config file is empty.");
            return;
        }

        logToBuffer("Loading config: ");
        logToBuffer(str);
        setSettingsFromJson(str);
    }

    void save() {
        if (!fsInitialized) {
            logToBuffer("LittleFS not initialized, cannot save.");
            return;
        }

        fs::File f = LittleFS.open(FILENAME, "w");
        if (!f) {
            logToBuffer("Failed to open config file for writing.");
            return;
        }

        String json = getSettingsJson();
        size_t written = f.print(json);
        f.flush();
        f.close();

        if (written > 0) {
            logToBuffer("Settings saved: ");
            logToBuffer(json);
            verifySave(json);
        } else {
            logToBuffer("Failed to write settings.");
        }
    }

    void verifySave(const String& expectedJson) {
        if (!LittleFS.exists(FILENAME)) {
            logToBuffer("VERIFY ERROR: File not found after save!");
            return;
        }

        fs::File f = LittleFS.open(FILENAME, "r");
        if (!f) {
            logToBuffer("VERIFY ERROR: Cannot open file for verification!");
            return;
        }

        String content = f.readString();
        f.close();

        if (content == expectedJson) {
            logToBuffer("VERIFY OK: File content matches.");
        } else {
            logToBuffer("VERIFY FAILED!");
            logToBuffer("Expected: ");
            logToBuffer(expectedJson);
            logToBuffer("Got: ");
            logToBuffer(content);
        }
    }

    String layoutToString(keyboard_utils::Layout layout) {
        switch (layout) {
            case keyboard_utils::LAYOUT_DE: return "DE";
            case keyboard_utils::LAYOUT_US: return "US";
            case keyboard_utils::LAYOUT_ES: return "ES";
            case keyboard_utils::LAYOUT_FR: return "FR";
            case keyboard_utils::LAYOUT_IT: return "IT";
            case keyboard_utils::LAYOUT_PT: return "PT";
            case keyboard_utils::LAYOUT_SE: return "SE";
            case keyboard_utils::LAYOUT_DK: return "DK";
            default: return DEFAULT_LAYOUT_STR;
        }
    }

    keyboard_utils::Layout stringToLayout(String layout_str) {
        if      (layout_str == "DE") return keyboard_utils::LAYOUT_DE;
        else if (layout_str == "US") return keyboard_utils::LAYOUT_US;
        else if (layout_str == "ES") return keyboard_utils::LAYOUT_ES;
        else if (layout_str == "FR") return keyboard_utils::LAYOUT_FR;
        else if (layout_str == "IT") return keyboard_utils::LAYOUT_IT;
        else if (layout_str == "PT") return keyboard_utils::LAYOUT_PT;
        else if (layout_str == "SE") return keyboard_utils::LAYOUT_SE;
        else if (layout_str == "DK") return keyboard_utils::LAYOUT_DK;
        else                         return keyboard_utils::LAYOUT_FR;
    }
    
    void logToBuffer(const char* log) {
        if (log == nullptr) return;
        logBuffer += "[LOG] ";
        logBuffer += log;
        logBuffer += "\n";
    }

    void logToBuffer(const String& log) {
        logBuffer += "[LOG] ";
        logBuffer += log;
        logBuffer += "\n";
    }

    void flushLog() {
        if (logBuffer.length() > 0) {
            Serial.print(logBuffer);
            logBuffer = "";
        }
    }
}