/**
 * @file    TEST_Error.cpp
 * @author  Marvin Smith
 * @date    7/8/2023
 */
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/outcome.hpp>

/****************************************/
/*          Test the Error Class        */
/****************************************/
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

TEST( Error, Get_String_Message_For_Invalid_Code )
{
    tmns::outcome::Error a { static_cast<TestErrorCode>(3) };
    EXPECT_EQ( a.message(), "unknown" );
}