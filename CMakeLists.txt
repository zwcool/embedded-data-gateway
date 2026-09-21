cmake_minimum_required(VERSION 3.20)
project(EmbeddedDataGateway VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# # Export compile_commands.json fuer clang-tidy, IDEs und Language Server
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Central compiler-warnings as interface-target
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wconversion -Wnon-virtual-dtor>
)

option(ENABLE_TESTS "Build unit tests" ON)

if(ENABLE_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googleTest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    
    set(INSTALL_GTEST OFF CACHE BOOl "" FORCE)
    FetchContent_MakeAvailable(googleTest)
endif()

add_subdirectory(gateway)
