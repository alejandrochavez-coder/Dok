cmake_minimum_required (VERSION 3.10)

project (dok_window_test
	LANGUAGES C
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../../)
add_executable (dok_window_test
	"main.c"
)

target_link_libraries(dok_window_test PRIVATE dok)