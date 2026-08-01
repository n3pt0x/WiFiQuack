#include <LittleFS.h>
#include <ArduinoJson.h>
#include "settings.h"
#include "debug.h"

namespace {
    String _wifi_ssid = "WiFiQuack";
    String _wifi_passphrase = "WiFiQuack";
    keyboard_utils::Layout _keyboard_layout = keyboard_utils::LAYOUT_FR;
    String _logBuffer;
    const char* _FILENAME = "/config.json";
    const char* _DEFAULT_LAYOUT_STR = "FR";
    bool _fsInitialized = false;
}

namespace settings {
    String getSettingsJson() {
        String json = "{";
        json += "\"wifi_ssid\":\"" + _wifi_ssid + "\",";
        json += "\"wifi_passphrase\":\"" + _wifi_passphrase + "\",";
        json += "\"keyboard_layout\":\"" + layoutToString(_keyboard_layout) + "\"";
        json += "}";
        return json;
    }

    bool setSettingsFromJson(const String& json) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, json);
        if (error) {
            logToBuffer(F("JSON parse error: "));
            logToBuffer(error.c_str());
            return false;
        }

        _wifi_ssid = doc["wifi_ssid"] | _wifi_ssid;
        _wifi_passphrase = doc["wifi_passphrase"] | _wifi_passphrase;
        String layout_str = doc["keyboard_layout"] | "FR";
        _keyboard_layout = stringToLayout(layout_str);
        return true;
    }

    bool begin() {
        #ifdef PLATFORM_ESP32
            if (!LittleFS.begin()) {
                logToBuffer(F("LittleFS mount failed. Attempting to format..."));
                
                if (!LittleFS.begin(true)) {
                    logToBuffer(F("LittleFS format failed. Using RAM only."));
                    _fsInitialized = false;
                    return false;
                }
                logToBuffer(F("LittleFS formatted successfully."));
            }
        #elifdef PLATFORM_PICO
            if (!LittleFS.begin()) {
                logToBuffer(F("LittleFS mount failed. Using RAM only."));
                _fsInitialized = false;
                return false;
            }
        #endif
        
        _fsInitialized = true;
        logToBuffer(F("LittleFS mounted successfully."));

        if (!LittleFS.exists(_FILENAME)) {
            logToBuffer(F("Config file not found. Creating default config..."));
            if (!createDefaultConfig()) {
                logToBuffer(F("Failed to create default config."));
                return false;
            }
        }

        load();
        return true;
    }

    bool createDefaultConfig() {
        fs::File configFile = LittleFS.open(_FILENAME, "w");
        if (!configFile) {
            logToBuffer(F("Failed to create config file."));
            return false;
        }

        String defaultConfig = getSettingsJson();
        size_t written = configFile.print(defaultConfig);
        configFile.flush();
        configFile.close();

        if (written == 0) {
            logToBuffer(F("Failed to write default config."));
            return false;
        }

        logToBuffer(F("Default config created: "));
        logToBuffer(defaultConfig);
        return true;
    }

    void load() {
        if (!_fsInitialized) {
            logToBuffer(F("LittleFS not initialized, cannot load."));
            return;
        }

        if (!LittleFS.exists(_FILENAME)) {
            logToBuffer(F("Config file does not exist."));
            return;
        }

        fs::File configFile = LittleFS.open(_FILENAME, "r");
        if (!configFile) {
            logToBuffer(F("Failed to open config file for reading."));
            return;
        }

        String str = configFile.readString();
        configFile.close();

        if (str.length() == 0) {
            logToBuffer(F("Config file is empty."));
            return;
        }

        logToBuffer(F("Loading config: "));
        logToBuffer(str);
        setSettingsFromJson(str);
    }

    void save() {
        if (!_fsInitialized) {
            logToBuffer(F("LittleFS not initialized, cannot save."));
            return;
        }

        fs::File f = LittleFS.open(_FILENAME, "w");
        if (!f) {
            logToBuffer(F("Failed to open config file for writing."));
            return;
        }

        String json = getSettingsJson();
        size_t written = f.print(json);
        f.flush();
        f.close();

        if (written > 0) {
            logToBuffer(F("Settings saved: "));
            logToBuffer(json);
            verifySave(json);
        } else {
            logToBuffer(F("Failed to write settings."));
        }
    }

    void verifySave(const String& expectedJson) {
        if (!LittleFS.exists(_FILENAME)) {
            logToBuffer(F("VERIFY ERROR: File not found after save!"));
            return;
        }

        fs::File f = LittleFS.open(_FILENAME, "r");
        if (!f) {
            logToBuffer(F("VERIFY ERROR: Cannot open file for verification!"));
            return;
        }

        String content = f.readString();
        f.close();

        if (content == expectedJson) {
            logToBuffer(F("VERIFY OK: File content matches."));
        } else {
            logToBuffer(F("VERIFY FAILED!"));
            logToBuffer(F("Expected: "));
            logToBuffer(expectedJson);
            logToBuffer(F("Got: "));
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
            default: return _DEFAULT_LAYOUT_STR;
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
        _logBuffer += "[LOG] ";
        _logBuffer += log;
        _logBuffer += "\n";
    }

    void logToBuffer(const String& log) {
        _logBuffer += "[LOG] ";
        _logBuffer += log;
        _logBuffer += "\n";
    }

    void flushLog() {
        if (_logBuffer.length() > 0) {
            debug(_logBuffer);
            _logBuffer = "";
        }
    }

    String getWiFiSSID() {
        return _wifi_ssid;
    }

    String getWiFiPassphrase() {
        return _wifi_passphrase;
    }

    keyboard_utils::Layout getKeyboardLayout() {
        return _keyboard_layout;
    }

    void setKeyboardLayout(keyboard_utils::Layout layout) {
        _keyboard_layout = layout;
    }
}