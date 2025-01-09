#ifndef EIR_LOGGER_HPP
#define EIR_LOGGER_HPP

/**
 * @file Logger.hpp
 * @author Johannes Halbach (halbachj@tcd.ie)
 * @brief Logger Base class
 * @version 0.1
 * @date 2024-01-08
 *
 * @copyright Copyright (c) 2023 EirSpace Rocketry
 *
 */

#include <cstdarg>
#include <cstdio>

/**
 * @brief Logger base for the flight computer
 */
template <typename Derived>
class Logger {
private:
    const int log_message_length = 256;
    const int log_prefix_length = 32;
    const int log_line_length = log_message_length + log_prefix_length;
    const char* name = "";

    void format(const char* level, const char* f_message, va_list args, char *log_buffer) {
        const char* clock = "RTC"; // TODO: get clock from RTC (clock maybe also singleton...)
        vsnprintf(&log_buffer[Logger::log_prefix_length], Logger::log_message_length, f_message, args); // write log line starting from 32
        snprintf(log_buffer, Logger::log_prefix_length, "[%s] [%s] [%s] - %s\n", clock, level, this->name); // write log prefix starting at start ending at 32
    }

    // Delete the copy constructor and assignment operator to prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

protected:
    static inline Derived* instance = nullptr;
    inline Logger(const char* name) : name(name) {} // Only allowed to be called by child classes
    ~Logger() = default; // No on else can kill this Singleton...

    virtual void writeLog(const char* message) = 0;

public:
    static inline Derived* get_logger(const char* name = __FILE__) {
        if (instance == nullptr) {
            instance = new Derived(name);
            Derived::init(); // Initialize special derived things
            // Cleanup memory when application terminates.
            // Singleton will exist for the lifetime of the program anyway
            // atexit(Derived::destroy);
        }
        return instance;
    }

    void info(const char* f_message, ...);
    void warning(const char* f_message, ...);
    void debug(const char* f_message, ...);
    void error(const char* f_message, ...);

};

#endif //EIR_LOGGER_HPP
