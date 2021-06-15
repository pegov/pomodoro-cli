#include "console.h"

#if defined (_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#define OS_WINDOWS

#include <windows.h>

inline void change_cursor_visibility(int show)
{
  HANDLE h_stdout;
  h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = show;
  SetConsoleCursorInfo(h_stdout, &info);
}

#else
#include <stdio.h>

#endif

void hide_cursor()
{
#ifdef OS_WINDOWS
  change_cursor_visibility(0);
#else
  printf("\e[?25l");
#endif
}

void show_cursor()
{
#ifdef OS_WINDOWS
  change_cursor_visibility(1);
#else
  printf("\e[?25h");
#endif
}

void clear_screen()
{
#ifdef OS_WINDOWS
  HANDLE h_stdout;
  h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  if (!GetConsoleMode(h_stdout, &mode))
      return;
  const DWORD original_mode = mode;
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(h_stdout, mode))
      return;
  DWORD written = 0;
  PCWSTR sequence = L"\033[2J\033[H";
  if (!WriteConsoleW(h_stdout, sequence, (DWORD)wcslen(sequence), &written, NULL))
  {
      SetConsoleMode(h_stdout, original_mode);
      return;
  }
  SetConsoleMode(h_stdout, original_mode);
#else
  printf("\e[1;1H\e[2J");
#endif
}