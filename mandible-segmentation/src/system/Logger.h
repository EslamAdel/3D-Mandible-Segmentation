#ifndef LOGGER_H
#define LOGGER_H

#include <LogLevel.hh>
#include <iostream>
#include <string>

/**
 * @brief The Logger class
 */
class Logger
{
public:
    /**
     * @brief instance
     * This function is called to create an instance of the class.
     * Calling the constructor publicly is not allowed.
     * The constructor is private and is only called by this Instance function.
     * @return
     */
    static Logger* instance();

    /**
     * @brief log
     * @param logLevel
     * @param filePath
     * @param lineNumber
     * @param functionName
     * @param string
     */
    void log( const LOG_LEVEL& logLevel,
              const std::string& filePath,
              const int &lineNumber,
              const std::string& functionName,
              const char *string, ... );

private:
    /**
     * @brief Logger
     */
    Logger();

private:
    /**
     * @brief operator =
     * @return
     */
    Logger& operator=( Logger const& );

private: // Private Member Variables
    /**
     * @brief instance_
     */
    static Logger* instance_;

    /**
     * @brief logFilePtr_
     * File stream or std stream
     */
    FILE* logFilePtr_;
};

/** \brief Log run time application information */
#define LOG_INFO( ARG... )                                                      \
    Logger::instance()->log( INFO, __FILE__, __LINE__, __FUNCTION__, ARG )

/** \brief Log debugging information */
#define LOG_DEBUG( ARG... )                                                     \
    Logger::instance()->log( DEBUG, __FILE__, __LINE__, __FUNCTION__, ARG )

/** \brief Log warning messages */
#define LOG_WARNING( ARG... )                                                   \
    Logger::instance()->log( WARNING, __FILE__, __LINE__, __FUNCTION__, ARG )

/** \brief Log error messages with out extra arguments */
#define LOG_ERROR( ARG... )                                                     \
    Logger::instance()->log( ERROR, __FILE__, __LINE__, __FUNCTION__, ARG )

#ifdef DEBUG_OPENGL_LOOP
/** \brief Log debugging information for OpenGL loop */
#define LOG_DEBUG_GL_LOOP( ARG... )                                             \
    Logger::instance()->log( DEBUG, __FILE__, __LINE__, __FUNCTION__, ARG )
#else
#define LOG_DEBUG_GL_LOOP( ARG... ) ( NULL )
#endif

#define LOG_EXIT( ARG... )                                                      \
    Logger::instance()->log( EXIT, __FILE__, __LINE__, __FUNCTION__, ARG )
#endif // LOGGER_H
