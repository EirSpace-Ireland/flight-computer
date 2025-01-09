#ifndef EIR_FILE_LOGGER_HPP
#define EIR_FILE_LOGGER_HPP

/**
 * @file FileLogger.hpp
 * @author Johannes Halbach (halbachj@tcd.ie)
 * @brief File Logger to log to a file
 * @version 0.1
 * @date 2024-01-09
 *
 * @copyright Copyright (c) 2023 EirSpace Rocketry
 *
 */

#include "Logger.hpp"
#include <sstream>

class FileLogger: public Logger<FileLogger> {
private:
    static inline std::stringstream* memory_stream = nullptr; // singleton attribute instantiated by init

protected:
    void writeLog(const char *message) override;

public:
    static void init();
    static void destroy();
    explicit FileLogger(const char* name) : Logger(name) {};

    static bool flush_to_SD();
};


#endif // EIR_FILE_LOGGER_HPP