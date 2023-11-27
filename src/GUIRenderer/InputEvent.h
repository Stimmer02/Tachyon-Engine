#ifndef INPUTEVENT_H
#define INPUTEVENT_H

enum Event{
  CLICK,
  PRESS,
  HOLD,
  MOVE
};

struct InputEvent
{
    int x;
    int y;
    Event event;
};


#endif