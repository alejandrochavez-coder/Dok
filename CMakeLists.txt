cmake_minimum_required (VERSION 3.10)
include(FetchContent)

project (dok
	LANGUAGES C
)

include(CTest)
add_library (dok STATIC
	"include/dok/dok_vulkan_tables.h"
	"include/dok/dok_core.h"
	"include/dok/dok_vector.h"
	src/dok_vulkan_tables.c
)
add_library (dok::dok ALIAS dok)

if (BUILD_TESTING)
	add_subdirectory(tests/vulkan)
	add_subdirectory(tests/dynamic_arrays)
	add_subdirectory(tests/window)
	add_test(NAME WindowTest COMMAND dok_window_test)
	add_test(NAME DynamicArraysTest COMMAND dok_dynamic_arrays_test)
	add_test(NAME VulkanTest COMMAND dok_vulkan_test)
endif()

target_include_directories(dok PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if (WIN32)
	target_link_libraries(dok PRIVATE user32)
	target_sources(dok PRIVATE 
		src/dok_core_win32.c
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