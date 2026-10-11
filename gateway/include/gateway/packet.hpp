#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace gateway {

struct TelemetryPacket {
    static constexpr std::size_t kSerializedSize = 8;

    uint32_t sequence_number{0};
    int32_t temperature_cent_deg{0};

    // Serialize packet content into Big-Endian (Network Byte Order)
    [[nodiscard]] std::array<std::byte, kSerializedSize> serialize() const noexcept;
};

}  // namespace gateway