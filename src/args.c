#include <stdlib.h>
#include <string.h>

void find_arg(int argc, char **argv, int *value, const char *short_name, const char *long_name) {
  // good enough
  // it handles only -a value and --arg value
  // TODO: handle --arg=value and -avalue
  int input;
  for (int i = 1; i < argc; i++) {
    if (strcmp(short_name, argv[i]) == 0 || strcmp(long_name, argv[i]) == 0) {
      if (argc >= i + 1) {
        *value = atoi(argv[i + 1]);
        if (input > 0)
          *value = input;
        return;
      }
    }
  }
}