#ifndef EVENTTYPE_H
#define EVENTTYPE_H

#define EVENT_STRINGS {"NONE", "HOVER", "CLICK", "TRIGGER", "RELEASE", "HOLD", "MOVE"}

enum EventType : char{
    NONE,
    ONHOVER,
    ONCLICK,
    ONTRIGGER,
    ONRELEASE,
    ONHOLD,
    ONMOVE,
    EVENT_COUNT
    //Etc...
};

#endif
