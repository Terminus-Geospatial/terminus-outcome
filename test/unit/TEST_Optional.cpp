/**
 * @file    TEST_Outcome.cpp
 * @author  Marvin Smith
 * @date    7/8/2023
 */
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/outcome.hpp>

/********************************************/
/*          Test the Optional Class         */
/********************************************/
TEST( Optional, DefaultConstructs )
{
    tmns::outcome::Optional<int> opt;
}

TEST( Optional, ValueConstructs )
{
    tmns::outcome::Optional<int> opt{42};
    EXPECT_EQ( opt.value(), 42 );
}

TEST( Optional, AssignEmpty )
{
    tmns::outcome::Optional<int> opt = tmns::outcome::cEmpty;
    EXPECT_FALSE( opt.has_value() );
}

TEST( Optional, Reference_Value )
{
    int v = 42;
    tmns::outcome::Optional<int&> opt { v };
    v = 24;
    EXPECT_EQ( opt.value(), 24 );
}

TEST( Optional, CopyConstructFromStandard )
{
    std::optional<int> stdopt{42};
    tmns::outcome::Optional<int> opt{stdopt};
    EXPECT_EQ( opt.value(), 42 );
    EXPECT_EQ( stdopt.value(), 42 );
}

TEST( Optional, MoveConstructFromStandard )
{
    std::optional<std::string> stdopt{ "Hello World" };
    tmns::outcome::Optional<std::string> opt{ std::move( stdopt ) };
    EXPECT_EQ( opt.value(), "Hello World" );
    EXPECT_TRUE( stdopt.value().empty() );
}

TEST( Optional, CopyConvertToStandard )
{
    tmns::outcome::Optional<int> opt { 42 };
    std::optional<int> stdopt = opt;
    EXPECT_EQ( opt.value(), 42 );
    EXPECT_EQ( stdopt.value(), 42 );
}

TEST( Optional, MoveConvertToStandard )
{
    tmns::outcome::Optional<std::string> opt{ "Hello World" };
    std::optional<std::string> stdopt = std::move( opt );
    EXPECT_EQ( stdopt.value(), "Hello World" );
    EXPECT_TRUE( opt.value().empty() );
}