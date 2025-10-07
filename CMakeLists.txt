cmake_minimum_required(VERSION 3.20)
project(TeamSpeakPlugin VERSION 1.4.0 LANGUAGES C CXX)

# Set C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Force use of release Python library even in debug builds
if(WIN32 AND MSVC)
    # Remove debug suffix from Python library
    set(Python3_LIBRARY_RELEASE "${Python3_LIBRARY_RELEASE}" CACHE STRING "Python release library" FORCE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Python3_LIBRARIES "${Python3_LIBRARY_RELEASE}")
    endif()
endif()

# Platform-specific settings
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${Python3_INCLUDE_DIRS}
)

# Plugin source files
set(PLUGIN_SOURCES
    src/core/plugin_main.c
    src/core/plugin_interface.c
    src/core/plugin_config.c
    src/events/connection_events.c
    src/events/channel_events.c
    src/events/client_events.c
    src/commands/command_handler.c
    src/ui/menu_handler.c
    src/ui/hotkey_handler.c
    src/utils/string_utils.c
    src/utils/logging.c
    src/python/python_engine.c
    src/python/python_api.c
    src/python/python_events.c
)

# Plugin header files
set(PLUGIN_HEADERS
    src/core/plugin_main.h
    src/core/plugin_interface.h
    src/core/plugin_config.h
    src/events/connection_events.h
    src/events/channel_events.h
    src/events/client_events.h
    src/commands/command_handler.h
    src/ui/menu_handler.h
    src/ui/hotkey_handler.h
    src/utils/string_utils.h
    src/utils/logging.h
    src/python/python_engine.h
    src/python/python_api.h
    src/python/python_events.h
    include/ts3_functions.h
    include/plugin_definitions.h
)

# Create the plugin library
add_library(${PROJECT_NAME} SHARED
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

# Set output name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "tspy_plugin"
    PREFIX ""
    SUFFIX ".dll"
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Python3_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Python3_LIBRARIES})
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /WX-
        /MP
        $<$<CONFIG:Release>:/O2 /GL>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/LTCG>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Release>:-O2>
    )
endif()

# Copy resources to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/icons
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/icons
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/icons
    COMMENT "Copying resources to output directory"
)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY resources/icons
    DESTINATION bin/icons
)
