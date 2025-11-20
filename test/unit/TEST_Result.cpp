/**
 * @file    TEST_Outcome.cpp
 * @author  Marvin Smith
 * @date    7/5/2023
*/
#include <gtest/gtest.h>

// Terminus Includes
#include <terminus/outcome.hpp>

// Local Includes
#include "TEST_error_code.hpp"

template <class ValueT>
using Result = tmns::outcome::Result<ValueT, TestErrorCode>;

/********************************************/
/*          Test the Result Class           */
/********************************************/
TEST( Result, BasicSuccess )
{
    Result<int> r = 42;
    EXPECT_TRUE( r.has_value() );
    EXPECT_EQ( r.assume_value(), 42 );
}

TEST( Result, BasicFailure )
{
    Result<int> r = tmns::outcome::fail( TestErrorCode::FIRST );
    EXPECT_TRUE( r.has_failure() );
    EXPECT_EQ( r.assume_error().code(), TestErrorCode::FIRST );
}

TEST( Result, BasicSuccessWithUtil )
{
    Result<int> r = tmns::outcome::ok<int>(42);
    EXPECT_TRUE( r.has_value() );
    EXPECT_EQ( r.assume_value(), 42 );
}

TEST( Result, BasicSuccessWithVoid )
{
    Result<void> r = tmns::outcome::ok();
    EXPECT_TRUE( r.has_value() );
}

TEST( Result, BasicFailureWithVoid )
{
    Result<void> r = tmns::outcome::fail( TestErrorCode::SECOND );
    EXPECT_TRUE( r.has_failure() );
    EXPECT_EQ( r.assume_error().code(), TestErrorCode::SECOND );
}

TEST( Result, ReferenceValue )
{
    int v = 42;
    Result<int&> r{v};
    v = 24;
    EXPECT_EQ( r.value(), 24 );
}

TEST( Result, TryMacroNoVariableSucceed )
{
    auto f = []() -> Result<void> { return tmns::outcome::ok(); };

    auto g = [&]() -> Result<void>{
        TERMINUS_OUTCOME_TRY(f());
        return tmns::outcome::ok();
    };

    auto r = g();
    EXPECT_TRUE( r.has_value() );
}

TEST( Result, TryMacroNoVariableFailure )
{
    auto f = []() -> Result<void> { return tmns::outcome::fail(TestErrorCode::FIRST); };

    auto g = [&]() -> Result<void>{
        TERMINUS_OUTCOME_TRY(f());
        return tmns::outcome::ok();
    };

    auto r = g();
    EXPECT_TRUE( r.has_error() );
    EXPECT_EQ( r.assume_error().code(), TestErrorCode::FIRST );
}

TEST( Result, TryMacroVariableSucceed )
{
    auto f = []() -> Result<int> { return 42; };

    auto g = [&]() -> Result<int>{
        TERMINUS_OUTCOME_TRY( auto v, f() );
        return v;
    };

    auto r = g();
    EXPECT_TRUE( r.has_value() );
    EXPECT_EQ( r.assume_value(), 42 );
}

TEST( Result, TryMacroVariableFailure )
{
    auto f = []() -> Result<int> { return tmns::outcome::fail(TestErrorCode::SECOND); };

    auto g = [&]() -> Result<int>{
        TERMINUS_OUTCOME_TRY( auto v, f() );
        return v;
    };

    auto r = g();
    EXPECT_TRUE( r.has_error() );
    EXPECT_EQ( r.assume_error().code(), TestErrorCode::SECOND );
}
