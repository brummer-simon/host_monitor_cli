
# Setup Project and Build
cmake_minimum_required(VERSION 3.16)

project(host_monitor_cli
    VERSION   1.0.0
    LANGUAGES CXX
)

# Specify source files
list(APPEND ${PROJECT_NAME}_SRC
    src/Args.cpp
    src/Config.cpp
    src/GroupElement.cpp
    src/ObserverElement.cpp
    src/UserInterface.cpp
    src/Util.cpp
    src/Version.cpp
    src/Window.cpp
    src/main.cpp
)

# Specify dependencies
find_library(HOST_MONITOR_LIB host_monitor)
find_library(NCURSES ncurses)

# Setup build
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    "${${PROJECT_NAME}_SRC}"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        VERSION_MINOR=${PROJECT_VERSION_MINOR}
        VERSION_PATCH=${PROJECT_VERSION_PATCH}
        VERSION=${PROJECT_VERSION}
)

target_compile_options(${PROJECT_NAME}
    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wtype-limits
        -Wstrict-overflow=5
        -fstrict-overflow
        -Wsign-compare
        -Wconversion
)

find_library(LIB_HOST_MONITOR host_monitor)
find_library(LIB_CURSES       ncurses)
find_library(LIB_PTHREAD      pthread)

target_link_libraries(${PROJECT_NAME}
    ${LIB_HOST_MONITOR}
    ${LIB_CURSES}
    ${LIB_PTHREAD}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        VERSION "${PROJECT_VERSION}"
)


# Setup deployment
install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        /usr/bin

    CONFIGURATIONS
        Release
)


