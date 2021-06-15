#include "events.h"
#include <stdint.h>

#if defined (_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#define OS_WINDOWS

#include <windows.h>
#include <conio.h>

#define getch() _getch()
#define usleep(x) Sleep(x)

#else

#include <unistd.h>
#include <pthread.h>
#include <curses.h>

#endif

static unsigned int c;

Flags new_flags() 
{
  Flags flags = {false, false, false, false};
  return flags;
}

#ifdef OS_WINDOWS
DWORD WINAPI event_function(void *params)
#else
void *event_function(void *params)
#endif
{
  Flags *flags;
  flags = (Flags *)params;

  while (1)
  {
    if ((c = getch()))
    {
      switch (c) {
        case 'q':
          flags->quit = true;
          break;
        case 'r':
          flags->reset = true;
          break;
        case 'g':
          flags->start = true;
          break;
        case 's':
          flags->stop = true;
          break;
      }
    }
    usleep(200);
  }
}

void create_event_handler(Flags *flags)
{
#ifdef OS_WINDOWS
  DWORD id;
  HANDLE thread = CreateThread(
    NULL,
    0,
    event_function,
    flags,
    0,
    &id
  );

  if (thread == NULL)
  {
    ExitProcess(3);
  }
#else
  pthread_t thread;
  int iret;

  iret = pthread_create(&thread, NULL, event_function, (void *) flags);
#endif

}