#include "gtest/gtest.h"
#include <gateway/version.hpp>

TEST(VersionTest, ReturnsCorrectInitialVersion) { EXPECT_EQ(gateway::get_version(), "0.0.1"); }