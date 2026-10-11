# Core business logic and data structures (platform-independent)
add_library(gateway_core
    src/version.cpp
    src/packet.cpp
)

target_enable_clang_tidy(gateway_core)

target_include_directories(gateway_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(gateway_core
    PUBLIC
        project_options
)

# Gateway application executable
add_executable(gateway_app
    src/main.cpp
)

target_link_libraries(gateway_app
    PRIVATE
        gateway_core
)

if(ENABLE_TESTS)
    add_subdirectory(test)
endif()