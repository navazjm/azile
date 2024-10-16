cmake_minimum_required(VERSION 3.10)
project(azile VERSION 0.1.0)

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_COMPILER /usr/bin/gcc)

### Build Static Lib ###
file(GLOB AZILE_SOURCES "src/*.c")
add_library(azile_lib STATIC ${AZILE_SOURCES})  # Create a static library
target_compile_options(azile_lib PRIVATE -Wall -Wextra -Werror)
target_include_directories(azile_lib PUBLIC src)

### Main Application Build ###
# add_executable(azile "src/azile.c")
# target_link_libraries(azile PRIVATE azile_lib)  # Link the library to the executable

### AddressSanitizer Build ###
# same as main tloc build, but attach AddressSanitizer flags to check for possible memory leaks
add_executable(azile "src/azile.c")
target_link_libraries(azile PRIVATE azile_lib)
target_compile_options(azile PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_options(azile PRIVATE -fsanitize=address)
set_target_properties(azile PROPERTIES
    COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer"
    LINK_FLAGS "-fsanitize=address"
)
