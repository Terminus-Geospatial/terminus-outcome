//**************************** INTELLECTUAL PROPERTY RIGHTS ****************************//
//*                                                                                    *//
//*                           Copyright (c) 2025 Terminus LLC                          *//
//*                                                                                    *//
//*                                All Rights Reserved.                                *//
//*                                                                                    *//
//*          Use of this source code is governed by LICENSE in the repo root.          *//
//*                                                                                    *//
//**************************** INTELLECTUAL PROPERTY RIGHTS ****************************//
/**
 * @file    TEST_error_code.cpp
 * @author  Marvin Smith
 * @date    7/8/2023
*/
#pragma once

// C++ Libraries
#include <optional>

// Terminus Libraries
#include <terminus/outcome.hpp>

enum class TestErrorCode { SUCCESS = 0, FIRST = 1, SECOND = 2 };

TERMINUS_OUTCOME_MARK_ENUM( TestErrorCode )

/**
 * @brief Error Category for TestErrorCode
 */
class outcome_Error_Category : public tmns::outcome::Error_Category<outcome_Error_Category>
{
    protected:
    
        const char* name_() const noexcept override
        {
            return "TestError";
        }

        std::string message_(int c) const override
        {
            switch( static_cast<TestErrorCode>(c) )
            {
                case TestErrorCode::SUCCESS:
                    return "success";
                case TestErrorCode::FIRST:
                    return "first";
                case TestErrorCode::SECOND:
                    return "second";
                default:
                    return "unknown";
            }
        }

}; // End of TestErrorCategory Class

TERMINUS_OUTCOME_ASSOCIATE_ERROR_CATEGORY( outcome_Error_Category, TestErrorCode );
