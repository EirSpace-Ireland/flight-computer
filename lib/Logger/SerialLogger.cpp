#include "SerialLogger.hpp"
#include <Arduino.h>

void SerialLogger::init() {
    if(!Serial) { // If not connected to Serial start connection
        Serial.begin(115200);
        while(!Serial); // wait to be connected. Will block until connection is established;
    }
}

void SerialLogger::destroy() {
    Serial.end();
}

void SerialLogger::writeLog(const char* message) {
    Serial.println(message);
}