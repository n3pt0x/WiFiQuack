#include <LittleFS.h>
#include "settings.h"
#include <ArduinoJson.h>

namespace settings {
    String wifi_ssid = "WiFiQuack";
    String wifi_passphrase = "WiFiQuack";
    keyboard_utils::Layout keyboard_layout = keyboard_utils::LAYOUT_FR;
    const char* FILENAME = "config.json";

    bool begin() {
        if (!LittleFS.begin()) {
            Serial.println("LittleFS mount failed.");
            return false;
        }

        if (!LittleFS.exists(FILENAME)) {
            fs::File configFile = LittleFS.open(FILENAME, "w");
            if (!configFile) {
                Serial.println("Failed to create config file.");
                return false;
            }

            String defaultConfig = getSettingsJson();

            size_t written = configFile.write(defaultConfig);
            configFile.close();

            if (written == 0) {
                Serial.println("Failed to write default config.");
                return false;
            }
        }

        load();

        return true;
    }

    void load() {
        fs::File configFile = LittleFS.open(FILENAME, "r");
        if (!configFile) {
            Serial.println("Failed to open config file for reading.");
            return;
        }

        String str = configFile.readString();
        configFile.close();

        if (str.length() == 0) {
            Serial.println("Config file is empty.");
            return;
        }

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, str);
        if (error) {
            Serial.print("JSON parse error: ");
            Serial.println(error.c_str());
            return;
        }

        wifi_ssid = doc["wifi_ssid"] | wifi_ssid;
        wifi_passphrase = doc["wifi_passphrase"] | wifi_passphrase;
        String layout_str = doc["keyboard_layout"] | DEFAULT_LAYOUT_STR;
        keyboard_layout = stringToLayout(layout_str);
    }

    void save() {
        fs::File f = LittleFS.open(FILENAME, "w");
        if (!f) return;

        String layoutStr = layoutToString(keyboard_layout);
    }

    String getSettingsJson() {
        String json = "{";
        json += "\"wifi_ssid\":" + wifi_ssid + "\"\", ";
        json += "\"wifi_passphrase\":" + wifi_passphrase + "\"\", ";
        json += "\"keyboard_layout\":" + layoutToString(keyboard_layout) + "\"\", ";
        json += "}";
        return json;
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
}