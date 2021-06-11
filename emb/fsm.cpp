#include "fsm.hpp"

#include "system.hpp"

using namespace System;

namespace FsmFramework {
Fsm::Fsm() : scheduler(NULL), next(NULL), parent(NULL) {}

void Fsm::execute() {
  if (next != 0) {
    (this->*next)();
  }
}
