#ifndef LOGGING_H
#define LOGGING_H

#include <string>

class Logger{
public:
  static int init();

  static void info(const char* fmt, ...);
  static void warning(const char* fmt, ...);
  static void error(const char* fmt, ...);
};

#endif //LOGGING_H