#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "system.hpp"

class LogInterface {
public:
  LogInterface();
  virtual ~LogInterface() {}

  void print(const char *str);
  void print(long i, const char *format = "%ld");

  static const char *endl;

  enum Level { Disable, Emergency, Error, Log } level;

protected:
  virtual void rawOutput(const char *str) = 0;
};

template <typename T> LogInterface &operator<<(LogInterface &os, const T &obj) {
  os.print(obj);
  return os;
}

extern LogInterface &Logger;

#define LOG_LOGGER_LEVEL(logger, msgLevel, msg)                                \
  if (logger.level >= msgLevel) {                                              \
    logger << Clock.millis() << " (" << __FILE__ << ":" << __LINE__ << ") "    \
           << msg << LogInterface::endl;                                       \
  }

#define LOG_LEVEL(msgLevel, msg) LOG_LOGGER_LEVEL(Logger, msgLevel, msg)

#define LOG(msg) LOG_LEVEL(LogInterface::Log, msg)

#define LOG_ERR(msg) LOG_LEVEL(LogInterface::Error, msg)

#define LOG_EMERG(msg) LOG_LEVEL(LogInterface::Emergency, msg)

#endif
