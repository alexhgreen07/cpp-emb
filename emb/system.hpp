#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

#include "list.hpp"
#include <stdlib.h>

namespace System {

class ClockInterface {
public:
  ClockInterface() {}
  virtual ~ClockInterface(){};
  virtual unsigned long millis() = 0;
  virtual unsigned long micros() = 0;
  virtual void delay(unsigned int ms) = 0;
  virtual void delayMicroseconds(unsigned int us) = 0;
};

struct SystemInterface {
  virtual ~SystemInterface() {}

  virtual void startCriticalSection() = 0;
  virtual void endCriticalSection() = 0;
};

}

#endif
