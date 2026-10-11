#include <bit>
#include <gateway/packet.hpp>

namespace gateway {

namespace {

// Serialize a 32-Bit unsigned integer to Big-Endian bytes
void serialize_u32_bigendian(uint32_t value, std::byte* destination) noexcept {
    destination[0] = static_cast<std::byte>((value >> 24) & 0xFF);
    destination[1] = static_cast<std::byte>((value >> 16) & 0xFF);
    destination[2] = static_cast<std::byte>((value >> 8) & 0xFF);
    destination[3] = static_cast<std::byte>(value & 0xFF);
}
}  // namespace

using SerializedBuffer = std::array<std::byte, TelemetryPacket::kSerializedSize>;

SerializedBuffer TelemetryPacket::serialize() const noexcept {
    std::array<std::byte, kSerializedSize> buffer{};

    serialize_u32_bigendian(sequence_number, buffer.data());

    // Safely reinterprets signed integer bit pattern to unsigned without
    // undefined behavior
    const auto kTempRaw = std::bit_cast<uint32_t>(temperature_cent_deg);
    serialize_u32_bigendian(kTempRaw, buffer.data() + sizeof(uint32_t));

    return buffer;
};
}  // namespace gateway