/****************************************************************************************************************************
Design and implement a log system with different log levels, which can output messages to console window:
- Support next log levels: info, debug, warning, error. The client of the code should not be able to define it's own log levels;
- Support logging to console window;
- Support multi-threaded environment, where several threads could log simultaneously;
- The client should be able to set a custom prefix, which appends automatically to each message;
- Each log message should start with date and time (no requirements to the format)

Log message format:
<Date>; <Log level>; <Prefix>(<Thread Id>): <Message>

Example of messages:
23/Feb/2021, 14:23:38; Info; Main(1256): This is my log message
23.02.2021 18:02:02; INFO; MyFunc(5628): Test message

Main.cpp file demonstrates the usage of the log system.
The task is to provide implementation (log.h and log.cpp), which can be used in attached Main.cpp file. Modification of main.cpp is forbidden.
Of course you can make more .h/.cpp files for your need, but please, do not send any other files than .h/.cpp files.
(e.g. don't send solutions, projects, executebles, object files etc)



FAQ:
- What OS should be used? Any. There is no need to use OS-specific code, so you can use any IDE/compiler/build automation or OS (but remember we develop only on Windows + VS in Fulcrum);
- What C++ standard is required? Not less than C++11 (main.cpp won't compile otherwise);
- Is the order of messages important? No, the order of messages from different threads is not important. More important not to break one message line or mix it with other messages;

Tips:
- There is an elagant implementation which doesn't use any synchronization (e.g. mutexes). But if you're struggling to come up with it - that's ok;
- Do testing. Don't limit yourself with our main.cpp. Try to change order of the threads, increase amount of log messages by 10 times, or add random sleeps\waits\yields.
***************************************************************************************************************************/



#include <thread>
#include "log.h"

/*
Example of console output:
23.02.2020 18:06:06; INFO; (1020): Starting the app
23.02.2020 18:06:06; DEBUG; (2561): Running a thread

Example of log.txt output:
23.02.2020 18:06:06; INFO; (2561): Running a thread
23.02.2020 18:06:06; DEBUG; f2(3444): Running a thread
23.02.2020 18:06:06; WARNING; f2(3444): Time spent in the thread: 10.0 seconds

Example of log2.txt:
23.02.2020 18:06:06; INFO; f3(3444): Running a thread
23.02.2020 18:06:06; ERROR; f3(3444): My int is 123;
*/


void f1() {
    auto logger1 = getLogger(); // Log to console
    // output: 23.02.2020 18:06:06; DEBUG; (2561): Running a thread
    logger1(DEBUG) << "Running a thread";

    auto logger2 = getLogger("f1"); // Each message has f1 prefix
    // output: 23.02.2020 18:06:06; INFO; (2561): Running a thread
    logger2(INFO) << "Running a thread" << " but log to another logger"; // Default log level INFO
}

void f2() {
    auto logger = getLogger("f2"); // Each message has f2 prefix
    // output: 23.02.2020 18:06:06; DEBUG; f2(3444): Running a thread
    logger(DEBUG) << "Running a thread";

    double timeSpent = 10.0;
    // output: 23.02.2020 18:06:06; WARNING; f2(3444): Time spent in the thread: 10.0 seconds
    logger(WARNING) << "Time spent in the thread: " << timeSpent << " seconds";
}

void f3() {
    auto logger = getLogger("f3");  // Each message has f3 prefix
    // output: 23.02.2020 18:06:06; INFO; f3(3444): Running a thread
    logger(INFO) << "Running a thread"; // Default log level INFO

    int i = 123;
    // output: 23.02.2020 18:06:06; ERROR; f3(3444): My int is 123;
    logger(ERROR) << "My int is " << i;
}

int main() {
    auto logger = getLogger("f111");
    // output: 23.02.2020 18:06:06; INFO; (1020): Starting the app
    logger(INFO) << "Starting the app"; // Default log level INFO

    std::thread t1(f1);
    std::thread t2(f2);
    std::thread t3(f3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
