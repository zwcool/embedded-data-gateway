cmake_minimum_required(VERSION 3.20)
project(EmbeddedDataGateway VERSION 0.1.0 LANGUAGES CXX)

# Export compile_commands.json for clangd, clang-tidy, and IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Compiler warnings configuration
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE 
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wconversion -Wnon-virtual-dtor>
)

# 2. General project compile settings (links warnings transitively)
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
target_link_libraries(project_options INTERFACE project_warnings)

option(ENABLE_TESTS "Build unit tests" ON)

if(ENABLE_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googleTest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    
    # Prevent GoogleTest from installing itself into the system
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googleTest)
endif()

add_subdirectory(gateway)