#include "SerialFileLogger.hpp"


void SerialFileLogger::init() {
    SerialLogger::init();
    FileLogger::init();
}

void SerialFileLogger::destroy() {
    SerialLogger::destroy();
    FileLogger::destroy();
}

void SerialFileLogger::writeLog(const char *message) {
    SerialLogger::writeLog(message); // Does this work?
    FileLogger::writeLog(message);
}
