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
 * @file    ErrorCategory.hpp
 * @author  Marvin Smith
 * @date    7/10/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/outcome/Error_Category.hpp>
#include <terminus/outcome/Result.hpp>
#include <terminus/outcome/macros.hpp>

// Terminus Image Libraries
#include "ErrorCode.hpp"

namespace tmns::core::error {

/**
 * Custom WarpCore error category for mapping all error codes to custom error types.
*/
class ErrorCategory : public tmns::outcome::ErrorCategory<ErrorCategory>
{
    protected:

        const char* name_() const noexcept override
        {
            return "Error";
        }

        std::string message_(int c) const override
        {
            switch( static_cast<ErrorCode>(c) )
            {
                case ErrorCode::SUCCESS:
                    return "SUCCESS";
                case ErrorCode::UNKNOWN:
                    return "UNKNOWN";
                case ErrorCode::ABORTED:
                    return "ABORTED";
                case ErrorCode::CONVERSION_ERROR:
                    return "CONVERSION_ERROR";
                case ErrorCode::DRIVER_FAILURE:
                    return "DRIVER_FAILURE";
                case ErrorCode::DRIVER_NOT_FOUND:
                    return "DRIVER_NOT_FOUND";
                case ErrorCode::DUPLICATE_FOUND:
                    return "DUPLICATE_FOUND";
                case ErrorCode::FEATURE_NOT_SUPPORTED:
                    return "FEATURE_NOT_SUPPORTED";
                case ErrorCode::FILE_IO_ERROR:
                    return "FILE_IO_ERROR";
                case ErrorCode::FILE_NOT_FOUND:
                    return "FILE_NOT_FOUND";
                case ErrorCode::GDAL_FAILURE:
                    return "GDAL_FAILURE";
                case ErrorCode::INCORRECT_SIZE:
                    return "INCORRECT_SIZE";
                case ErrorCode::INVALID_CHANNEL_TYPE:
                    return "INVALID_CHANNEL_TYPE";
                case ErrorCode::INVALID_CONFIGURATION:
                    return "INVALID_CONFIGURATION";
                case ErrorCode::INVALID_INPUT:
                    return "INVALID_INPUT";
                case ErrorCode::INVALID_PIXEL_TYPE:
                    return "INVALID_PIXEL_TYPE";
                case ErrorCode::INVALID_SIZE:
                    return "INVALID_SIZE";
                case ErrorCode::ISIS_ERROR:
                    return "ISIS_ERROR";
                case ErrorCode::NOT_FOUND:
                    return "NOT_FOUND";
                case ErrorCode::NOT_IMPLEMENTED:
                    return "NOT_IMPLEMENTED";
                case ErrorCode::NOT_SUPPORTED:
                    return "NOT_SUPPORTED";
                case ErrorCode::OPENJPEG_FAILURE:
                    return "OPENJPEG_FAILURE";
                case ErrorCode::OUT_OF_BOUNDS:
                    return "OUT_OF_BOUNDS";
                case ErrorCode::OUT_OF_MEMORY:
                    return "OUT_OF_MEMORY";
                case ErrorCode::PARSING_ERROR:
                    return "PARSING_ERROR";
                case ErrorCode::UNDEFINED:
                    return "UNDEFINED";
                case ErrorCode::UNINITIALIZED:
                    return "UNINITIALIZED";
                default:
                    return "UNKNOWN";
            }
        }
}; // End of ErrorCategory class

// Associate the custom error category with your custom error code
TERMINUS_OUTCOME_ASSOCIATE_ERROR_CATEGORY( ErrorCategory, ErrorCode )

} // End of tmns::core::error namespace

// Define a convenient alias for the `Result` type that uses your error codes
template <class ValueT>
using Result = tmns::outcome::Result<ValueT,tmns::core::error::ErrorCode>;
