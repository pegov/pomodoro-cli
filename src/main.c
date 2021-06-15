#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "args.h"
#include "console.h"
#include "events.h"

#if defined (_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)

#include <windows.h>

#define sleep(x) Sleep(x * 1000)
#define usleep(x) Sleep(x)
#define beep() Beep(500, 1000)

#else
#include <unistd.h>
#include <termios.h>

#define beep()

#endif

#define BLOCKS_BEFORE_LONG_BREAK 4
#define SMALL_BREAK 10
#define LONG_BREAK 25
#define WORK 50

#define PD_ACTION_SB 0
#define PD_ACTION_LB 1
#define PD_ACTION_W 2

void sig_handler(int signum)
{
  show_cursor();
  raise(SIGINT);
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);
  atexit(show_cursor);

  int blocks = BLOCKS_BEFORE_LONG_BREAK;
  int small_break = SMALL_BREAK;
  int long_break = LONG_BREAK;
  int work = WORK;

  if (argc > 2) {
    find_arg(argc, argv, &blocks, "-b", "--blocks");
    find_arg(argc, argv, &small_break, "-s", "--small");
    find_arg(argc, argv, &long_break, "-l", "--long");
    find_arg(argc, argv, &work, "-w", "--work");
  }

  hide_cursor();
  clear_screen();
  
  printf("Settings:\n");
  printf("Blocks: %d\n", blocks);
  printf("Small break: %d minutes\n", small_break);
  printf("Long break: %d minutes\n", long_break);
  printf("Work: %d minutes\n", work);
  printf("Press ENTER to start\n");

  getchar();

  small_break = small_break * 60;
  long_break = long_break * 60;
  work = work * 60;

#ifdef DEBUG
  small_break = 5;
  long_break = 10;
  work = 8;
#endif

  time_t cur_time, stop_time, delta_time, plus_time;
  int minutes_left, seconds_left;

  cur_time = time(NULL);
  plus_time = work;
  stop_time = cur_time + plus_time;
  delta_time = plus_time;

  int cur_block = 0;
  int total_blocks = 0;
  int cur_action = PD_ACTION_W;

  Flags flags = new_flags();
  create_event_handler(&flags);

  while (1) {
    while (delta_time > 0) {
      usleep(500);

      cur_time = time(NULL);

      if (flags.quit) {
        exit(EXIT_SUCCESS);
      }
      if (flags.reset) {
        stop_time = cur_time + plus_time;
        flags.reset = false;
        flags.stop = false;
        flags.start = false;
        continue;
      }
      if (flags.start) {
        flags.start = false;
        flags.stop = false;
        continue;
      }
      if (flags.stop) {
        stop_time = cur_time + delta_time;
        continue;
      }

      delta_time = stop_time - cur_time;
      if (delta_time < 0)
        break;
      minutes_left = delta_time / 60;
      seconds_left = delta_time % 60;


      clear_screen();
      printf("Total blocks: %d\n", total_blocks);
      printf("Event: ");
      switch (cur_action) {
        case PD_ACTION_SB:
          printf("Small break\n");
          break;
        case PD_ACTION_LB:
          printf("Long break\n");
          break;
        case PD_ACTION_W:
          printf("Work\n");
          printf("Current block: %d out of %d\n", cur_block + 1, blocks);
          break;
      }
      printf("Time left: %02d:%02d\n", minutes_left, seconds_left);
      printf("q - quit\nr - reset\ng - start\ns - stop\n");
    }

    switch (cur_action) {
      case PD_ACTION_SB:
      case PD_ACTION_LB:
        plus_time = work;
        cur_action = PD_ACTION_W;
        break;
      case PD_ACTION_W:
        cur_block++;
        total_blocks++;
        if (cur_block >= blocks) {
          cur_action = PD_ACTION_LB;
          plus_time = long_break;
          cur_block = 0;
        } else {
          cur_action = PD_ACTION_SB;
          plus_time = small_break;
        }
    }

    // beep();
    cur_time = time(NULL);
    stop_time = cur_time + plus_time;
    delta_time = stop_time - cur_time;
  }

  return EXIT_SUCCESS;
}