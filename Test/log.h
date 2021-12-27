#pragma once
#include <string>
#include <fstream>
#include <functional>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

std::ostream& operator<<(std::ostream& os, const LogLevel& data);

extern const std::string DEBUG;
extern const std::string INFO;
extern const std::string WARNING;
extern const std::string ERROR;

class Logger
{
private:
    enum class OutputTarget
    {
        STDOUT,
        FILE
    };

    OutputTarget target;
    LogLevel logLevel;
    std::ofstream logFile;

    std::string prefix;
    std::string logText;
    std::thread::id threadID;

    short int response;
    bool checkFirstCall;

    void setToStdout();
    void setToFile(const std::string& fileName);
    void setLogLevel(LogLevel logLevel);
    void log(LogLevel logLevel, const std::string& text);
    void ask();
    void chooseWhereToWrite();
    void chooseLogLevel(const std::string& str);
    std::string getCurrentTime();

public:
    Logger(const std::string& prefix, const std::string& level);

    Logger& operator<< (const char* text);
    Logger& operator<< (const int number);
};

std::function<Logger(const std::string)> getLogger(const std::string prefix = "");