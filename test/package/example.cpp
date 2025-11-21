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
 * @file    example.cpp
 * @author  Marvin Smith
 * @date    7/8/2023
 */

// Terminus Libraries
#include <terminus/outcome.hpp>

// C++ Libraries
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// 1 - Define a custom error code enumeration
enum class IoErrorCode { SUCCESS = 0,
                         BAD_READ = 1,
                         BAD_WRITE = 2,
                         NOT_FOUND = 3 };

// 2 - Mark the error code type so that the library knows it is part of the framework
TERMINUS_OUTCOME_MARK_ENUM( IoErrorCode )

// 3 - Define a custom error category that can properly interpret the integer representation of
//     error codes from your custom error code type.
class IoErrorCategory : public tmns::outcome::Error_Category<IoErrorCategory>
{
    protected:

        const char* name_() const noexcept override
        {
            return "IoError";
        }

        std::string message_(int c) const override
        {
            switch( static_cast<IoErrorCode>(c) )
            {
                case IoErrorCode::SUCCESS:
                    return "success";
                case IoErrorCode::BAD_READ:
                    return "bad read";
                case IoErrorCode::BAD_WRITE:
                    return "bad write";
                case IoErrorCode::NOT_FOUND:
                    return "not found";
                default:
                    return "unknown";
            }
        }
}; // End of IoErrorCategory class

// 4 - Associate the custom error category with your custom error code
TERMINUS_OUTCOME_ASSOCIATE_ERROR_CATEGORY( IoErrorCategory, IoErrorCode )

// 5 - (optional) Define a convenient alias for the `Result` type that uses your error codes
template <class ValueT>
using IoResult = tmns::outcome::Result<ValueT,IoErrorCode>;

/**
 * SImple function return a result by reference
*/
IoResult<int> some_func_01()
{
    return tmns::outcome::ok<int>( 3 );
}

struct Point
{
    double x;
    double y;
};

IoResult<std::shared_ptr<Point>> some_func_02()
{
    auto pt = std::make_shared<Point>();
    pt->x = 1;
    pt->y = 2;
    return tmns::outcome::ok<std::shared_ptr<Point>>( std::move( pt ) );
}

int main()
{
    IoResult<std::string> readResult = tmns::outcome::ok<std::string>( "Hello World!" );

    if( readResult.has_error() )
    {
        std::cerr << readResult.assume_error() << std::endl;
        return 1;
    }
    std::cout << readResult.assume_value() << std::endl;

    tmns::outcome::Optional<std::string> oMessage{ "Hello World" };
    if( oMessage.has_value() )
    {
        std::cout << *oMessage << std::endl;
    }

    // Test functions
    std::cout << "Function 1: " << some_func_01().assume_value() << std::endl;
    std::cout << "Function 2: " << some_func_02().assume_value()->x << std::endl;

    // Get build information and log this
    auto build_info = tmns::outcome::get_build_info();
    std::stringstream ss;
    ss << "Terminus Outcome Build Information:\n";
    for( const auto& [key, value] : build_info )
    {
        ss << key << ": " << value << "\n";
    }
    std::cout << ss.str() << std::endl;

    return 0;
}