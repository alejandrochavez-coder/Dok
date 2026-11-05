cmake_minimum_required (VERSION 3.11)

include(FetchContent)

project (dok_vulkan_test
	LANGUAGES C
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
	cglm
	GIT_REPOSITORY	https://github.com/recp/cglm.git
)

FetchContent_MakeAvailable(cglm)

include_directories(../../)
add_executable (dok_vulkan_test
	"main.c"
)

target_link_libraries(dok_vulkan_test PRIVATE dok cglm)