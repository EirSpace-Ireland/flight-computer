#include "Logger.hpp"


template<typename Derived>
void Logger<Derived>::info(const char* f_message, ...) {
    va_list args;
    va_start (args, f_message);
    char log_line_buffer[Logger::log_line_length]; // string where full log line is stored

    format("INFO", f_message, args, log_line_buffer);

    writeLog(log_line_buffer);

    va_end (args);
}

template<typename Derived>
void Logger<Derived>::debug(const char* f_message, ...) {
    va_list args;
    va_start (args, f_message);

    char log_line_buffer[Logger::log_line_length]; // string where full log line is stored
    format("DEBUG", f_message, args, log_line_buffer);

    writeLog(log_line_buffer);

    va_end (args);
}

template<typename Derived>
void Logger<Derived>::warning(const char* f_message, ...) {
    va_list args;
    va_start (args, f_message);

    char log_line_buffer[Logger::log_line_length]; // string where full log line is stored
    format("WARNING", f_message, args, log_line_buffer);

    writeLog(log_line_buffer);

    va_end (args);
}

template<typename Derived>
void Logger<Derived>::error(const char* f_message, ...) {
    va_list args;
    va_start (args, f_message);

    char log_line_buffer[Logger::log_line_length]; // string where full log line is stored
    format("ERROR", f_message, args, log_line_buffer);

    writeLog(log_line_buffer);

    va_end (args);
}