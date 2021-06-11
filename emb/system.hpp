#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

#include "list.hpp"
#include <stdlib.h>

namespace System {

class ClockInterface {
public:
  ClockInterface() {}
  virtual ~ClockInterface(){};
  virtual unsigned long millis();
  virtual unsigned long micros();
  virtual void delay(unsigned int ms);
  virtual void delayMicroseconds(unsigned int us);
};

struct SystemInterface {
  virtual ~SystemInterface() {}

  virtual void startCriticalSection();
  virtual void endCriticalSection();
};

extern ClockInterface Clock;
extern SystemInterface System;
}

#endif
