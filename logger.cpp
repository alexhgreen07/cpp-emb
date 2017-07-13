#include "logger.hpp"

#include <stdio.h>
#include <string.h>

const char *LogInterface::endl = "\r\n";

LogInterface::LogInterface() : level(Log) {}

void LogInterface::print(const char *str) { rawOutput(str); }

void LogInterface::print(long l, const char *format) {
  char str[33];
  sprintf(str, format, l);
  print(str);
}
