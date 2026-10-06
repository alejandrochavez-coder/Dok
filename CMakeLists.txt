cmake_minimum_required (VERSION 3.10)
include(FetchContent)

project (dok
	LANGUAGES C
)

add_library (dok STATIC dok.c dok.h)
add_library (dok::dok ALIAS dok)

target_include_directories(dok PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:include>
)

if (NOT WIN32)
	target_link_libraries(dok PUBLIC ${CMAKE_DL_LIBS})
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