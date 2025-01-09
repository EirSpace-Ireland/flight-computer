#ifndef EIR_SERIAL_LOGGER_HPP
#define EIR_SERIAL_LOGGER_HPP

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

class SerialLogger: public Logger<SerialLogger> {
protected:
    void writeLog(const char* message) override;
public:
    /**
     * @brief Has to be called in setup to instantiate the Serial interface.
     *        Will block until Serial is connected. Without this the first
     *        call to get_logger will fail.
     * @return bool optional success info, can be ignored
     */
    static void init();
    static void destroy();

    explicit SerialLogger(const char* name) : Logger(name) {}
};

#endif // EIR_SERIAL_LOGGER_HPP