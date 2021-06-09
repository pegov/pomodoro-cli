CC=clang
CFLAGS=
SOURCES=src/main.c
EXECUTABLE=pomodoro.exe

all: clean build

build:
	$(CC) $(CFLAGS) ${SOURCES} -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)