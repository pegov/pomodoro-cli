#ifndef EVENTS_H
#define EVENTS_H

#include <stdbool.h>

typedef struct Flags {
  bool quit;
  bool reset;
  bool start;
  bool stop;
} Flags;

Flags new_flags();

void create_event_handler(Flags *flags);

#endif // EVENTS_H