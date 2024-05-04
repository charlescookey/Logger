
/*********************************************************************
 * @file  Logger.h
 *
 * @brief Implementation of the class Logger.
 *********************************************************************/


#pragma once
#include <string>
#include <cstdio>
#include <mutex>
#include <ctime>


//needed to use fopen if  using visual studio
#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif


class Logger{
public:
    /**
    * The different channels currently supported.
    *
    */
    enum LogChannel {
        FileChannel , ConsoleChannel
    };

    /**
    * The different orders currently supported.
    *
    */
    enum LogOrder {
        LogDebug, LogInfo , LogError
    };


    /**
    * Set the file path and name to which the log output would be written.
    *
    * @param new_filepath   the filepath
    * @param micros the microseconds fraction
    *
    * @return true if the file exists or was created otherwise it returns false
    */
    static bool setFilepath(const char* new_filepath) {
        return getInstance().init(new_filepath);
    }

    /**
    * Writes the log messge for the specifiesd order to the specified channel .
    *
    * @param channel   the channel to wwrite the  log to, file or console
    * @param Order   the log order, debug, info, error
    * @param logMessage   the message to be logged in string format
    *
    */
    static void Log(LogChannel channel, LogOrder order, std::string logMessage) {
        getInstance().LOG(channel, order, logMessage.c_str());
    }

    /**
    * Writes the log messge for the specifiesd order to the specified channel .
    *
    * @param channel   the channel to wwrite the  log to, file or console
    * @param Order   the log order, debug, info, error
    * @param logMessage   the message to be logged in c-string format
    * @param args   the arguments that can be passed into the c-string
    *
    */
    template<typename... Args>
    static void Log(LogChannel channel, LogOrder order, const char* logMessage, Args... args) {
        getInstance().LOG(channel, order, logMessage, args...);
    }

private:

    std::mutex logMutex;
    char _time[80];
    const char* timestampFormat = "%d-%m-%Y : %T";
    std::FILE* file = 0;
    const char* filepath = "log.txt";

    Logger() {}

    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    ~Logger()
    {
        deleteFile();
    }

    static Logger& getInstance()
    {
        static Logger instance;

        return instance;
    }

    const char* orderString(LogOrder order) {
        if (order == LogDebug) {
            return "[Debug]     ";
        }
        else if (order == LogInfo) {
            return "[Info]      ";
        }
        else if (order == LogError) {
            return "[Error]     ";
        }
        else {
            return "[****]      ";
        }
    }

    /**
    * The default initializer that creates or chheck if the file exists.
    *
    */

    bool init(){
        deleteFile();
        file = std::fopen(filepath, "a");

        if (file == 0)
        {
            return false;
        }
        return true;
    }

    /**
    * uses the new filepath passed in to create or chheck if the file exists.
    *
    * @param new_filepath    the new filepath to write to
    *
    */

    bool init(const char* new_filepath){
        deleteFile();
        file = std::fopen(new_filepath, "a");

        if (file == 0)
        {
            return false;
        }
        return true;
    }

    void deleteFile()
    {
        if (file)
        {
            std::fclose(file);
            file = 0;
        }
    }

    template<typename... Args>
    void fileLog(const char* time, const char* messageOrderStr, const char* logMessage, Args... args) {
        if (file)
        {
            std::fprintf(file, "%s    ", time);
            std::fprintf(file, messageOrderStr);
            std::fprintf(file, logMessage, args...);
            std::fprintf(file, "\n");
        }
        else {
            if (getInstance().init())
                getInstance().fileLog(time, messageOrderStr, logMessage, args...);
        }
    }

    template<typename... Args>
    void consoleLog(const char* time, const char* messageOrderStr, const char* logMessage, Args... args) {
        std::printf("%s    ", time);
        std::printf(messageOrderStr);
        std::printf(logMessage, args...);
        std::printf("\n");
    }

    /**
    * The functions calls the appropriate log function based on the channel pprovided.
    *
    * @param channel    the channel to log  to
    * @param Order      the order of the log, debug, error or info
    * @param message    the message to log
    * @param args       the arguments to pass into the message
    *
    */

    template<typename... Args>
    void LOG(LogChannel channel, LogOrder order, const char* message, Args... args) {
        std::time_t current_time = std::time(0);
        std::tm* timestamp = std::localtime(&current_time);

        std::scoped_lock lock(logMutex);
        std::strftime(_time, 80, timestampFormat, timestamp);

        if (channel == FileChannel) {
            getInstance().fileLog(_time, orderString(order), message, args...);
        }
        else if (channel == ConsoleChannel) {
            getInstance().consoleLog(_time, orderString(order), message, args...);
        }
    }
};

//https://charlescookey.com/