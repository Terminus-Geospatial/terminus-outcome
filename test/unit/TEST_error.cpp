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
 * @file    TEST_error.cpp
 * @author  Marvin Smith
 * @date    7/8/2023
 */

// GoogleTest Libraries
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/outcome.hpp>

// Local Test Utilities
#include "TEST_error_code.hpp"

/****************************************/
/*          Test the Error Class        */
/****************************************/
/**
 * @test Formats the composed message from various inputs and categories.
 */
TEST( Error, Get_String_Message )
{
    tmns::outcome::Error a { TestErrorCode::FIRST, "message number ", 42 };
    EXPECT_EQ( a.code(), TestErrorCode::FIRST );
    EXPECT_EQ( a.code().category().name(), "TestError" );
    EXPECT_EQ( a.details(), "message number 42" );

    tmns::outcome::Error b { TestErrorCode::SUCCESS };
    EXPECT_EQ( b.message(), "success" );

    tmns::outcome::Error c { TestErrorCode::SECOND, "another ", "error ", "message" };
    EXPECT_EQ( c.message(), "second: another error message" );

}

/**
 * @test Falls back to the category's unknown string when the code is invalid.
 */
TEST( Error, Get_String_Message_For_Invalid_Code )
{
    tmns::outcome::Error a { static_cast<TestErrorCode>(3) };
    EXPECT_EQ( a.message(), "unknown" );
}