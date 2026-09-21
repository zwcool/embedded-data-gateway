add_library(gateway_lib
    src/version.cpp
)

target_include_directories(gateway_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(gateway_lib
    PUBLIC
        project_warnings
)

add_executable(gateway_app
    src/main.cpp
)

target_link_libraries(gateway_app
    PRIVATE
        gateway_lib
)

if(ENABLE_TESTS)
    add_subdirectory(test)
endif()