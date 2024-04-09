#ifndef ILOG_H
#define ILOG_H

#include "MessageType.h"

class ILog {
public:
  virtual void Write(enum MessageType _type, const char * _format, ...) = 0;
};

#endif
