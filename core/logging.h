#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include <mutex>

class Logger{
public:
  static int init();

  static void info(const char* fmt, ...);
  static void warning(const char* fmt, ...);
  static void error(const char* fmt, ...);
private:
  static std::mutex _mutex; // Mutex for thread safety
};

#endif //LOGGING_H