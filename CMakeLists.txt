cmake_minimum_required (VERSION 3.10)
include(FetchContent)

project (dok
LANGUAGES C
)

include(CTest)
add_library (dok STATIC
	"include/dok/dok_vulkan_tables.h"
	"include/dok/dok.h"
	"include/dok/dok_window.h"
	include/dok/dok_vulkan_loader.h
)
add_library (dok::dok ALIAS dok)

if (BUILD_TESTING)
	add_subdirectory(test)
	add_test(NAME DokTest COMMAND dok_test)
endif()

target_include_directories(dok PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if (WIN32)
	target_link_libraries(dok PRIVATE user32)
	target_sources(dok PRIVATE 
		src/dok_window_win32.c
	    src/dok_vulkan_loader_win32.c
	)
endif()


find_package(Vulkan QUIET)

if (Vulkan_INCLUDE_DIRS)
	target_include_directories(dok PUBLIC ${Vulkan_INCLUDE_DIRS})
elseif(DEFINED ENV{VULKAN_SDK})
    target_include_directories(dok PUBLIC "$ENV{VULKAN_SDK}/include")
elseif(TARGET Vulkan-Headers)
	target_link_libraries(dok PUBLIC Vulkan::Headers)
else ()
	FetchContent_Declare(
		vulkan-headers
		GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
	)
	FetchContent_MakeAvailable(vulkan-headers)
	target_link_libraries(dok PUBLIC Vulkan::Headers)
endif()