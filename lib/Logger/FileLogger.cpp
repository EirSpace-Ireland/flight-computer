#include "FileLogger.hpp"

void FileLogger::init() {
    FileLogger::memory_stream = new std::stringstream();
}

bool FileLogger::flush_to_SD() {
    // TODO: Flush memory stream to SD
    return false;
}

void FileLogger::writeLog(const char *message) {
    *memory_stream << message << std::endl;
}

void FileLogger::destroy() {
    delete FileLogger::memory_stream; // destroy stream when object is destroyed
}


