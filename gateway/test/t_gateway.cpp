#include <gateway/version.hpp>

#include "gtest/gtest.h"

TEST(VersionTest, ReturnsCorrectInitialVersion) { EXPECT_EQ(gateway::get_version(), "0.0.1"); }