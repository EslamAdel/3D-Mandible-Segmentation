#ifndef LOG_LEVEL_HH
#define LOG_LEVEL_HH

enum LOG_LEVEL
{
    INFO,           // Informative
    DEBUG,          // Debugging
    WARNING,        // Warning
    ERROR,          // Error
    DEBUG_GL_LOOP,  // Logged within an OpenGL rendering loop 'performance'
    EXIT
};

#endif // LOG_LEVEL_HH

