/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    TEST_optional.cpp
 * @author  Marvin Smith
 * @date    7/8/2023
 */

// C++ Standard Libraries
#include <optional>
#include <string>

// GoogleTest Libraries
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/outcome.hpp>

/********************************************/
/*          Test the Optional Class         */
/********************************************/
/**
 * @test Verifies the optional default constructor produces an empty value.
 */
TEST( Optional, DefaultConstructs )
{
    tmns::outcome::Optional<int> opt;
}

/**
 * @test Validates direct value construction stores the payload.
 */
TEST( Optional, ValueConstructs )
{
    tmns::outcome::Optional<int> opt{42};
    EXPECT_EQ( opt.value(), 42 );
}

/**
 * @test Ensures the legacy empty tag constructs a disengaged optional.
 */
TEST( Optional, AssignEmpty )
{
    tmns::outcome::Optional<int> opt = tmns::outcome::cEmpty;
    EXPECT_FALSE( opt.has_value() );
}

/**
 * @test Confirms reference optionals reflect mutations to the referent.
 */
TEST( Optional, Reference_Value )
{
    int v = 42;
    tmns::outcome::Optional<int&> opt { v };
    v = 24;
    EXPECT_EQ( opt.value(), 24 );
}

/**
 * @test Copies from a std::optional while leaving the source untouched.
 */
TEST( Optional, CopyConstructFromStandard )
{
    std::optional<int> stdopt{42};
    tmns::outcome::Optional<int> opt{stdopt};
    EXPECT_EQ( opt.value(), 42 );
    EXPECT_EQ( stdopt.value(), 42 );
}

/**
 * @test Moves from a std::optional and empties the source instance.
 */
TEST( Optional, MoveConstructFromStandard )
{
    std::optional<std::string> stdopt{ "Hello World" };
    tmns::outcome::Optional<std::string> opt{ std::move( stdopt ) };
    EXPECT_EQ( opt.value(), "Hello World" );
    EXPECT_TRUE( stdopt.value().empty() );
}

/**
 * @test Synchronizes from a std::optional via copy assignment and detects resets.
 */
TEST( Optional, CopyAssignFromStandard )
{
    tmns::outcome::Optional<int> opt;
    std::optional<int> stdopt{ 99 };
    opt = stdopt;
    EXPECT_TRUE( opt.has_value() );
    EXPECT_EQ( opt.value(), 99 );

    stdopt.reset();
    opt = stdopt;
    EXPECT_FALSE( opt.has_value() );
}

/**
 * @test Synchronizes from a std::optional via move assignment.
 */
TEST( Optional, MoveAssignFromStandard )
{
    tmns::outcome::Optional<std::string> opt;
    std::optional<std::string> stdopt{ "Goodbye" };
    opt = std::move( stdopt );
    EXPECT_TRUE( opt.has_value() );
    EXPECT_EQ( opt.value(), "Goodbye" );
    EXPECT_TRUE( stdopt->empty() );

    stdopt.reset();
    opt = std::move( stdopt );
    EXPECT_FALSE( opt.has_value() );
}

/**
 * @test Converts to std::optional by copy.
 */
TEST( Optional, CopyConvertToStandard )
{
    tmns::outcome::Optional<int> opt { 42 };
    std::optional<int> stdopt = opt;
    EXPECT_EQ( opt.value(), 42 );
    EXPECT_EQ( stdopt.value(), 42 );
}

/**
 * @test Converts to std::optional by move.
 */
TEST( Optional, MoveConvertToStandard )
{
    tmns::outcome::Optional<std::string> opt{ "Hello World" };
    std::optional<std::string> stdopt = std::move( opt );
    EXPECT_EQ( stdopt.value(), "Hello World" );
    EXPECT_TRUE( opt.value().empty() );
}