#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <Logger.h>
#include <Colors.hh>

#define TIME_STAMP_CHAR_LENGTH 128

// Global static pointer used to ensure a single instance of the class
Logger* Logger::instance_ = NULL;

Logger::Logger()
{
    // Print the log messages to the std stream
    logFilePtr_ = stdout;
}

Logger* Logger::instance()
{
    if ( !instance_ )
        instance_ = new Logger;

    return instance_;
}

void Logger::log( const LOG_LEVEL& logLevel,
                  const std::string& filePath,
                  const int& lineNumber,
                  const std::string& functionName,
                  const char* string, ... )
{
    // Variable arguments information
    va_list argumentList;

    // Lof message
    char logMessage[1024];

    // Get the arguments and add them to the buffer
    va_start( argumentList, string );
    vsnprintf( logMessage, sizeof( logMessage ), string, argumentList );

    // Get the time now
    time_t timeNow = time( NULL );

    // Time stamp string
    char timeStamp[TIME_STAMP_CHAR_LENGTH];

    // Format the time string and get the local time
    strftime( timeStamp, sizeof(timeStamp ),
              ( char* )"%D.%M.%Y, %H:%M:%S", localtime( &timeNow ));

    switch ( logLevel )
    {
        case INFO:
            fprintf( logFilePtr_,
                     STD_RED "[%d]" STD_RESET                   // Process Id
                     STD_YELLOW "[%s]" STD_RESET                // Time stamp
                     STD_CYAN " %s :" STD_RESET                 // filePath name
                     STD_GREEN "[%d]\n" STD_RESET               // Code line
                     STD_MAGENTA "\t* %s" STD_RESET             // Function name
                     STD_WHITE " %s \n" STD_RESET,              // Message
                     (int) getpid(),                            // Process Id
                     timeStamp,                                 // Time stamp
                     filePath.c_str(),                          // filePath name
                     lineNumber,                                // Code line
                     functionName.c_str(),                      // Function name
                     logMessage);                               // Message
            break;

        case DEBUG:
            fprintf( logFilePtr_,
                     STD_RED "[%d]" STD_RESET                   // Process Id
                     STD_YELLOW "[%s]" STD_RESET                // Time stamp
                     STD_CYAN " %s :" STD_RESET                 // filePath name
                     STD_GREEN "[%d] \n" STD_RESET              // Code line
                     STD_MAGENTA "\t* %s" STD_RESET             // Function name
                     STD_BOLD_WHITE " %s \n" STD_RESET,         // Message
                     (int) getpid(),                            // Process Id
                     timeStamp,                                 // Time stamp
                     filePath.c_str(),                          // filePath name
                     lineNumber,                                // Code line
                     functionName.c_str(),                      // Function name
                     logMessage);                               // Message
            break;

        case WARNING:
            fprintf( logFilePtr_,
                     STD_RED "[%d]" STD_RESET                   // Process Id
                     STD_YELLOW "[%s]" STD_RESET                // Time stamp
                     STD_CYAN " %s :" STD_RESET                 // filePath name
                     STD_GREEN "[%d] " STD_RESET                // Code line
                     STD_BOLD_YELLOW "[WARNGIN] \n" STD_RESET   // [WARNING]
                     STD_MAGENTA "\t* %s" STD_RESET             // Function name
                     STD_BOLD_YELLOW " %s \n" STD_RESET,        // Message
                     (int) getpid(),                            // Process Id
                     timeStamp,                                 // Time stamp
                     filePath.c_str(),                          // filePath name
                     lineNumber,                                // Code line
                     functionName.c_str(),                      // Function name
                     logMessage);                               // Message
            break;

        case ERROR:
            fprintf( logFilePtr_,
                     STD_RED "[%d]" STD_RESET                   // Process Id
                     STD_YELLOW "[%s]" STD_RESET                // Time stamp
                     STD_CYAN " %s :" STD_RESET                 // filePath name
                     STD_GREEN "[%d] " STD_RESET                // Code line
                     STD_BOLD_RED "[ERROR] \n" STD_RESET        // [ERROR]
                     STD_MAGENTA "\t* %s" STD_RESET             // Function name
                     STD_BOLD_RED " %s \n" STD_RESET,           // Message
                     (int) getpid(),                            // Process Id
                     timeStamp,                                 // Time stamp
                     filePath.c_str(),                          // filePath name
                     lineNumber,                                // Code line
                     functionName.c_str(),                      // Function name
                     logMessage);                               // Message
            LOG_EXIT(logMessage);
            break;

        case EXIT:
            fprintf( logFilePtr_,
                     STD_BOLD_RED "\t EXITTING due to an ERROR ...\n " STD_RESET );
            exit(EXIT_FAILURE);
            break;

        default:
            break;
    }

    // Done
    va_end(argumentList);
}
