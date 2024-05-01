#ifndef ILOG_H
#define ILOG_H

#include "MessageType.h"

class ILog {
public:

  virtual void Write(enum LogMessageType _type, const char * _format, ...) = 0;

  virtual void Flush() = 0;

  virtual ~ILog(){}

};

#endif
