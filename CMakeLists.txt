cmake_minimum_required(VERSION 3.10)
project(azile VERSION 0.1.0)

set(CMAKE_DEBUG_POSTFIX _d)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# TODO: fetch libgit2 if not already installed
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGIT2 REQUIRED libgit2)
include_directories(${LIBGIT2_INCLUDE_DIRS})
link_directories(${LIBGIT2_LIBRARY_DIRS})

### Main Application Build ###
file(GLOB AZILE_SOURCES "src/*.c")
add_library(azile_lib STATIC ${AZILE_SOURCES})  # Create a static library
target_compile_options(azile_lib PRIVATE -Wall -Wextra -Werror)
target_include_directories(azile_lib PUBLIC src)
add_executable(azile "src/azile.c")
target_link_libraries(azile PRIVATE azile_lib ${LIBGIT2_LIBRARIES})  # Link the library to the executable
set_target_properties(azile PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

install(TARGETS azile DESTINATION /usr/local/bin)
