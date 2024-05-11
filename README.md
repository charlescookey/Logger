# Logger
* [MY PORTTFOLIO](https://charlescookey.com/)

## Basic Logger.
Task Description:

Create a universal trace library for application event logging to different channels. The library should support logging to at least one of the following channels: TCP, serial, console, file, etc. Additionally, it should be designed in a non-blocking manner to ensure it does not impede the normal operation of the applications utilizing it.

Requirements:

1. Implement a C++ library that enables application event logging.
2. The library should support logging to at least one of the following channels: TCP, serial, console, file, etc. You may choose the channel that best suits your expertise or demonstrate versatility by implementing logging to multiple channels.
3. Ensure the library is non-blocking to prevent it from interfering with the normal execution of applications.
4. Provide clear documentation on how to use the library and its different features.
5. Optionally, you may include unit tests to ensure the reliability and robustness of your implementation.


### How to Use
This is a header-only library. Copy [Logger.h](Logger.h) file and `#include` it in your project.

### How to log
Here is an example
```cpp
#include "Logger.h"

int main()
{
    std::string a = "string";
    std::string str = "Test Log with std::string";
    std::string result;

    Logger::Log(Logger::ConsoleChannel , Logger::LogDebug, "Test Log with number %d" , 5);
    Logger::Log(Logger::ConsoleChannel , Logger::LogInfo, "Test Log with c-string %s , %i" , a.c_str(), 15);
    Logger::Log(Logger::ConsoleChannel , Logger::LogError, str);


    Logger::setFilepath("output.txt");

    Logger::Log(Logger::FileChannel , Logger::LogDebug, str);
    Logger::Log(Logger::FileChannel, Logger::LogInfo, "Test Log with c-string %s , %i" , a.c_str(), 15);

}
```
Output:
> 03-05-2024 : 11:46:30    [Debug]     Test Log with number 5
> 03-05-2024 : 11:46:30    [Info]      Test Log with c-string string , 15
> 03-05-2024 : 11:46:30    [Error]     Test Log with std::string

and file output

> 03-05-2024 : 11:46:30    [Debug]     Test Log with std::string
> 03-05-2024 : 11:46:30    [Info]      Test Log with c-string string , 15


###  Ease if use
No initialization, just use log functions

### Log Channels
Theses are the channels currently supported
```cpp
	Logger::FileChannel
    Logger::ConsoleChannel
```

### Log Orders
Theses are the orders currently supported
```cpp
	Logger::LogDebug
    Logger::LogInfo
    Logger::LogError
```
  
### File Output
To set the file path file output, call
```cpp
	Logger::setFilepath("output.txt");
```
if no file path is set the default file location is `"log.txt"`.
  
