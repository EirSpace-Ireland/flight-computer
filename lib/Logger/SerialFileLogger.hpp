

#include "SerialLogger.hpp"
#include "FileLogger.hpp"


class SerialFileLogger: public SerialLogger, public FileLogger {
protected:
    void writeLog(const char* message) override;
public:
    static void init();
    static void destroy();

    explicit SerialFileLogger(const char* name): SerialLogger(name), FileLogger(name) {}

};