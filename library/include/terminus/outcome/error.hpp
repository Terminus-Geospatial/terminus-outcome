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
 * @file    Error.hpp
 * @author  Marvin Smith
 * @date    7/7/2023
*/
#pragma once

// C++ Includes
#include <iostream>
#include <sstream>
#include <system_error>

namespace tmns::outcome {

/**
 * Error the occurred during an operation.
 *
 * This templated type associates any custom, enumerated error code with a string message providing
 * additional information about the error.  The optional detailed message can be created by passing
 * any number of arguments to the constructor after the `ErrorCodeT` value.  All arguments after the
 * first will be converted to a string and concatenated together to form the final error message.  The
 * only requirement is that the values being written are writable to a `std::ostream` as text.
 *
 * @tparam ErrorCodeT The enumeration type containing the supported the supported set of error codes.
*/
template <class ErrorCodeT>
class Error
{
    public:

        /**
         * @brief Constructs an error message for the instance using the provided arguments.
         *
         * The parameter pack `args` can contain values of any type that can be written to a
         * `std::ostream`.
         *
         * @param ec  The error code indicating what error occurred.
         * @param args The arguments used to compose the detailed error message.
         */
        template <class... ArgsT>
        Error( ErrorCodeT ec, ArgsT&&... args )
          : m_ecode{ec},
            m_details{ static_cast<std::ostringstream&&>((std::ostringstream{} << ... << args)).str() }
        {}

        /**
         * @brief Return the error code indicating what error occurred.
         * @returns Error code that occurred based on ErrorCodeT
         */
        const std::error_code& code() const
        {
            return m_ecode;
        }

        /**
         * @brief Return the message with additional details about the context of the error.
         * @returns The message with additional details.
         */
        const std::string& details() const
        {
            return m_details;
        }

        /**
         * @brief Return the full message with the error.
         * @returns The full message
         */
        std::string message() const
        {
            auto m = m_ecode.message();
            if( !m_details.empty() )
            {
                m += ": " + m_details;
            }
            return m;
        }

        /**
         * @brief Append additional information to the error message.
        */
        Error& append( const std::string& details )
        {
            m_details += ": " + details;
            return *this;
        }

        /**
         * @brief Implicit string conversion operator
         */
        operator std::string() const
        {
            return message();
        }

    private:

        /// Error code
        std::error_code m_ecode;

        /// Message with additional details
        std::string m_details;

}; // End of Error Class

/**
 * Overload of the ADL-dependent free function that can create a standard library error code
 * from our custom error object.
 */
template <class ErrorCodeT>
inline const std::error_code& make_error_code( const Error<ErrorCodeT>& e )
{
    return e.code();
}

/**
 * Output stream operator that formats the error message and details when writing to
 * a stream.
*/
template <class ErrorCodeT>
inline std::ostream& operator << ( std::ostream& s, const Error<ErrorCodeT>& e )
{
    s << e.message();
    return s;
}

} // End of tmns::outcome namespace