cmake_minimum_required(VERSION 3.20)
project(EmbeddedDataGateway VERSION 0.1.0 LANGUAGES CXX)

# Option via CMake command to set clang-tidy
option(ENABLE_CLANG_TIDY "Enable static code analysis with clang-tidy" ON)

# Export compile_commands.json for clangd, clang-tidy, and IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings configuration
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE 
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wconversion -Wnon-virtual-dtor>
)

# Enable clang-tidy if option is set
if(ENABLE_CLANG_TIDY)
    message(STATUS "clang-tidy is ON")

    find_program(CLANG_TIDY_EXE "clang-tidy")
    if(CLANG_TIDY_EXE)
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    else()
        message(WARNING "clang-tidy not found!")
    endif() 
else()
    message(STATUS "clang-tidy disabled!")
endif()

function(target_enable_clang_tidy TARGET_NAME)
    if(ENABLE_CLANG_TIDY AND CLANG_TIDY_EXE)
        set_target_properties(${TARGET_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endfunction()

# Clang-Format Integration
find_program(CLANG_FORMAT_EXE "clang-format")
if(CLANG_FORMAT_EXE)
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")

    # Collect only internal project sources (exclude build/ and FetchContent)
    file(GLOB_RECURSE PROJECT_FORMAT_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/gateway/*.[ch]pp"
        "${CMAKE_CURRENT_SOURCE_DIR}/gateway/*.[ch]"
    )

    if(PROJECT_FORMAT_SOURCES)
        # Apply formatting in-place
        add_custom_target(format
            COMMAND "${CLANG_FORMAT_EXE}" -i ${PROJECT_FORMAT_SOURCES}
            COMMENT "Applying clang-format to project sources"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            VERBATIM
        )

        # Dry-run check for CI pipelines (returns non-zero if formatting is needed)
        add_custom_target(format-check
            COMMAND "${CLANG_FORMAT_EXE}" --dry-run --Werror ${PROJECT_FORMAT_SOURCES}
            COMMENT "Verifying code formatting with clang-format"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            VERBATIM
        )
    endif()
else()
    message(STATUS "clang-format not found - format targets will not be available")
endif()

# General project compile settings (links warnings transitively)
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