cmake_minimum_required (VERSION 3.10)

project (dok_vulkan_test
	LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(../../)
add_executable (dok_vulkan_test
	"main.c"
)

target_link_libraries(dok_vulkan_test PRIVATE dok)