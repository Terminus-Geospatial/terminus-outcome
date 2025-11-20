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
 * @file    TEST_result.cpp
 * @author  Marvin Smith
 * @date    7/5/2023
*/

// GoogleTest Libraries
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/outcome.hpp>

// Local Test Utilities
#include "TEST_error_code.hpp"

template <class ValueT>
using Result = tmns::outcome::Result<ValueT, TestErrorCode>;

/********************************************/
/*          Test the Result Class           */
/********************************************/
/**
 * @test Stores a successful integer payload via implicit construction.
 */
TEST( Result, BasicSuccess )
{
    Result<int> r = 42;
    EXPECT_TRUE( r.has_value() );
    EXPECT_EQ( r.assume_value(), 42 );
}

/**
 * @test Captures an error through the failure helper.
 */
TEST( Result, BasicFailure )
{
    Result<int> r = tmns::outcome::fail( TestErrorCode::FIRST );
    EXPECT_TRUE( r.has_failure() );
    EXPECT_EQ( r.assume_error().code(), TestErrorCode::FIRST );
}

/**
 * @test Uses the ok() helper to forward constructor arguments.
 */
TEST( Result, BasicSuccessWithUtil )
{
    Result<int> r = tmns::outcome::ok<int>(42);
    EXPECT_TRUE( r.has_value() );
    EXPECT_EQ( r.assume_value(), 42 );
}

/**
 * @test Confirms void specializations use ok().
 */
TEST( Result, BasicSuccessWithVoid )
{
    Result<void> r = tmns::outcome::ok();
    EXPECT_TRUE( r.has_value() );
}

/**
 * @test Captures error codes for void specializations.
 */
TEST( Result, BasicFailureWithVoid )
{
    Result<void> r = tmns::outcome::fail( TestErrorCode::SECOND );
    EXPECT_TRUE( r.has_failure() );
    EXPECT_EQ( r.assume_error().code(), TestErrorCode::SECOND );
}

/**
 * @test Ensures reference payloads observe mutations.
 */
TEST( Result, ReferenceValue )
{
    int v = 42;
    Result<int&> r{v};
    v = 24;
    EXPECT_EQ( r.value(), 24 );
}

/**
 * @test `TERMINUS_OUTCOME_TRY(expr)` returns early on success without binding a variable.
 */
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

/**
 * @test `TERMINUS_OUTCOME_TRY(expr)` returns early on failure without binding a variable.
 */
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

/**
 * @test `TERMINUS_OUTCOME_TRY(var, expr)` binds success values when the computation succeeds.
 */
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

/**
 * @test `TERMINUS_OUTCOME_TRY(var, expr)` returns failures before the bound variable is used.
 */
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
