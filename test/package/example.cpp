/**
 * @file    example.cpp
 * @author  Marvin Smith
 * @date    7/8/2023
 */

// Terminus Libraries
#include <terminus/outcome/ErrorCategory.hpp>
#include <terminus/outcome/Optional.hpp>
#include <terminus/outcome/Result.hpp>
#include <terminus/outcome/macros.hpp>

// C++ Libraries
#include <fstream>
#include <iostream>
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
class IoErrorCategory : public tmns::outcome::ErrorCategory<IoErrorCategory>
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

    return 0;
}