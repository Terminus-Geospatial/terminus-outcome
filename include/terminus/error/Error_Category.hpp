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
 * @file    Error_Category.hpp
 * @author  Marvin Smith
 * @date    7/10/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/outcome/Error_Category.hpp>
#include <terminus/outcome/Result.hpp>
#include <terminus/outcome/macros.hpp>

// Terminus Image Libraries
#include "Error_Code.hpp"

namespace tmns::core::error {

/**
 * Custom WarpCore error category for mapping all error codes to custom error types.
*/
class Error_Category : public tmns::outcome::Error_Category<Error_Category>
{
    protected:

        const char* name_() const noexcept override
        {
            return "Error";
        }

        std::string message_(int c) const override
        {
            switch( static_cast<Error_Code>(c) )
            {
                case Error_Code::SUCCESS:
                    return "SUCCESS";
                case Error_Code::UNKNOWN:
                    return "UNKNOWN";
                case Error_Code::ABORTED:
                    return "ABORTED";
                case Error_Code::CONVERSION_ERROR:
                    return "CONVERSION_ERROR";
                case Error_Code::DRIVER_FAILURE:
                    return "DRIVER_FAILURE";
                case Error_Code::DRIVER_NOT_FOUND:
                    return "DRIVER_NOT_FOUND";
                case Error_Code::DUPLICATE_FOUND:
                    return "DUPLICATE_FOUND";
                case Error_Code::FEATURE_NOT_SUPPORTED:
                    return "FEATURE_NOT_SUPPORTED";
                case Error_Code::FILE_IO_ERROR:
                    return "FILE_IO_ERROR";
                case Error_Code::FILE_NOT_FOUND:
                    return "FILE_NOT_FOUND";
                case Error_Code::GDAL_FAILURE:
                    return "GDAL_FAILURE";
                case Error_Code::INCORRECT_SIZE:
                    return "INCORRECT_SIZE";
                case Error_Code::INVALID_CHANNEL_TYPE:
                    return "INVALID_CHANNEL_TYPE";
                case Error_Code::INVALID_CONFIGURATION:
                    return "INVALID_CONFIGURATION";
                case Error_Code::INVALID_INPUT:
                    return "INVALID_INPUT";
                case Error_Code::INVALID_PIXEL_TYPE:
                    return "INVALID_PIXEL_TYPE";
                case Error_Code::INVALID_SIZE:
                    return "INVALID_SIZE";
                case Error_Code::ISIS_ERROR:
                    return "ISIS_ERROR";
                case Error_Code::NOT_FOUND:
                    return "NOT_FOUND";
                case Error_Code::NOT_IMPLEMENTED:
                    return "NOT_IMPLEMENTED";
                case Error_Code::NOT_SUPPORTED:
                    return "NOT_SUPPORTED";
                case Error_Code::OPENJPEG_FAILURE:
                    return "OPENJPEG_FAILURE";
                case Error_Code::OUT_OF_BOUNDS:
                    return "OUT_OF_BOUNDS";
                case Error_Code::OUT_OF_MEMORY:
                    return "OUT_OF_MEMORY";
                case Error_Code::PARSING_ERROR:
                    return "PARSING_ERROR";
                case Error_Code::UNDEFINED:
                    return "UNDEFINED";
                case Error_Code::UNINITIALIZED:
                    return "UNINITIALIZED";
                default:
                    return "UNKNOWN";
            }
        }
}; // End of ErrorCategory class

// Associate the custom error category with your custom error code
TERMINUS_OUTCOME_ASSOCIATE_ERROR_CATEGORY( Error_Category, Error_Code )

} // End of tmns::core::error namespace

// Define a convenient alias for the `Result` type that uses your error codes
template <class ValueT>
using Result = tmns::outcome::Result<ValueT,tmns::core::error::Error_Code>;
