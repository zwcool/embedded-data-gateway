#pragma once
#include <string_view>

namespace gateway {

[[nodiscard]] constexpr std::string_view get_version() noexcept { return "0.0.1"; }

} // namespace gateway