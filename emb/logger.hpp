/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "system.hpp"

namespace emb
{

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

}

#endif
