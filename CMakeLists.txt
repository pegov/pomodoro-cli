cmake_minimum_required(VERSION 3.5)

project (pomodoro)

add_executable(pomodoro 
	src/args.c
	src/console.c
	src/events.c
	src/main.c
)

target_include_directories(pomodoro
	PRIVATE
		${PROJECT_SOURCE_DIR}/src
)

if (UNIX)
target_link_libraries(pomodoro pthread ncurses)
endif (UNIX)