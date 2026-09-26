add_executable(test_gateway
    t_gateway.cpp
    t_packet.cpp
)

target_link_libraries(test_gateway
    PRIVATE
        gateway_core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_gateway)