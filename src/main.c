#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


#if defined (_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#define OS_WINDOWS
#endif

#ifdef OS_WINDOWS
#include <windows.h>

#define clrscr() system("cls")
#define sleep(x) Sleep(x * 1000)
#define beep() Beep(500, 1000)


#else
#include <unistd.h>
#include <term.h>

#define clrscr() system("clear")
#define beep()

#endif

#define BLOCKS_BEFORE_LONG_BREAK 4
#define SMALL_BREAK 10
#define LONG_BREAK 25
#define WORK 50

#define PD_ACTION_SB 0
#define PD_ACTION_LB 1
#define PD_ACTION_W 2


void parse_args(int argc, char **argv, int *value, int default_value, const char *short_name, const char *long_name)
{
  // TODO: handle --arg=value
  for (int i = 1; i < argc; i++) {
    if (strcmp(short_name, argv[i]) == 0 || strcmp(long_name, argv[i]) == 0) {
      if (argc >= i + 1) {
        *value = atoi(argv[i+1]);
        if (*value <= 0) {
          *value = default_value;
        }
      }
    }
  }
  if (!*value)
    *value = default_value;
}

int main(int argc, char **argv)
{
  int blocks, small_break, long_break, work;
  if (argc > 2) {
    parse_args(argc, argv, &blocks, BLOCKS_BEFORE_LONG_BREAK, "-b", "--blocks");
    parse_args(argc, argv, &small_break, SMALL_BREAK, "-s", "--small");
    parse_args(argc, argv, &long_break, LONG_BREAK, "-l", "--long");
    parse_args(argc, argv, &work, WORK, "-w", "--work");
  } else {
    blocks = BLOCKS_BEFORE_LONG_BREAK;
    small_break = SMALL_BREAK;
    long_break = LONG_BREAK;
    work = WORK;
  }
  
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
  stop_time = cur_time + work;

  int cur_block = 0;
  int total_blocks = 0;
  int cur_action = PD_ACTION_W;

  char *minutes_prefix;
  char *seconds_prefix;

  while (TRUE) {
    while (cur_time < stop_time) {
      clrscr();
      cur_time = time(NULL);
      
      delta_time = stop_time - cur_time;
      if (delta_time < 0)
        delta_time = 0;
      minutes_left = delta_time / 60;
      seconds_left = delta_time % 60;

      if (minutes_left < 10) {
        minutes_prefix = "0";
      } else {
        minutes_prefix = "";
      }
      
      if (seconds_left < 10) {
        seconds_prefix = "0";
      } else {
        seconds_prefix = "";
      }

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

      printf("Time left: %s%d:%s%d\n", minutes_prefix, minutes_left, seconds_prefix, seconds_left);
      sleep(1);
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

    beep();
    printf("Done!\n");
    printf("Press ENTER to continue\n");
    getchar();
    cur_time = time(NULL);
    stop_time = cur_time + plus_time;
  }

  return EXIT_SUCCESS;
}