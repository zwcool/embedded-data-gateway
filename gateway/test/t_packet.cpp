#include <array>
#include <cstddef>
#include <cstdint>
#include <gateway/packet.hpp>
#include <gtest/gtest.h>

TEST(PacketTest, SerializeCorrectlyToBigEndian) {
    // 1. Arrange: Create packet with known values
    // 0x01020304 -> Sequence 1690960
    // 0x00000866 -> Temperature 2150 (21.5 C)
    const gateway::TelemetryPacket packet{.sequence_number = 0x01020304,
                                          .temperature_cent_deg = 2150};

    // 2. Act Serialize to byte buffer
    const auto buffer = packet.serialize();

    // 3. Assert: Validate exact byte layout on the wire (Big-Endian)
    const std::array<std::byte, 8> expected = {
        std::byte{0x01}, std::byte{0x02}, std::byte{0x03}, std::byte{0x04}, // Sequence
        std::byte{0x00}, std::byte{0x00}, std::byte{0x08}, std::byte{0x66}  // Temperature
    };

    EXPECT_EQ(buffer, expected);
}