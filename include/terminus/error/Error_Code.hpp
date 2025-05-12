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
 * @file    Error_Code.hpp
 * @author  Marvin Smith
 * @date    7/10/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/outcome/macros.hpp>

namespace tmns::core::error {

/**
 * Custom ErrorCode enumeration for all WarpCore issues.
*/
enum class ErrorCode { SUCCESS               =  0,
                       UNKNOWN               =  1,
                       ABORTED               =  2,
                       CONVERSION_ERROR      =  3,
                       DRIVER_NOT_FOUND      =  4,
                       DRIVER_FAILURE        =  5 /**< Catch-all for general external library issues. */,
                       DUPLICATE_FOUND       =  6,
                       FEATURE_NOT_SUPPORTED =  7,
                       FILE_IO_ERROR         =  8,
                       FILE_NOT_FOUND        =  9,
                       GDAL_FAILURE          = 10,
                       INCORRECT_SIZE        = 11,
                       INVALID_CHANNEL_TYPE  = 12,
                       INVALID_CONFIGURATION = 13,
                       INVALID_INPUT         = 14,
                       INVALID_PIXEL_TYPE    = 15,
                       INVALID_SIZE          = 16,
                       ISIS_ERROR            = 17,
                       NOT_FOUND             = 18,
                       NOT_IMPLEMENTED       = 19,
                       NOT_SUPPORTED         = 20,
                       OPENJPEG_FAILURE      = 21 /**< Catch-all issue for OpenJPEG API. */,
                       OUT_OF_BOUNDS         = 22,
                       OUT_OF_MEMORY         = 23,
                       PARSING_ERROR         = 24,
                       UNDEFINED             = 25,
                       UNINITIALIZED         = 26,
};

} // End of tmns::core::error namespace

// Mark the error code type so that the library knows it is part of the framework
TERMINUS_OUTCOME_MARK_ENUM( tmns::core::error::ErrorCode )