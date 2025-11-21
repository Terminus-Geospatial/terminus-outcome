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
 * @date    7/7/2023
*/
#pragma once

#include <string>
#include <system_error>

namespace tmns::outcome {

/**
 * Abstract implementation of a generic error category.  This implementation allows our errors to be
 * used by many standard library utilities and any other libs/apps that utility `std::error_code`
 * for error handling.
 *
 * When creating a unique error category for your library or application, you should create a new
 * type that inherits from this class and provides implementations for the protected `name_()` and
 * `message_()` functions.
 *
 * A program only needs one instance of the `ErrorCategory`, so this type also sets up you
 * derived type as a singleton.  Calling the static public member function `get()` on the derived
 * type will give back the singleton category for that type.
 *
 * As long as you call the `TERMINUS_OUTCOME_ASSOCIATE_ERROR_CATEGORY` macro with your derived
 * category and your custom error code enumeration, you shouldn't need to worry about
 * anything else besides providing implementations for the `name_()` and `message_()` functions
 * in your derived class.
 *
 * @tparam DerivedErrorCategoryT
 *          This type is needed so the static public `get()` function provides access to
 *          the singleton instance of your derived class.  This pattern is called CRTP.
 */
template <class DerivedErrorCategoryT>
class Error_Category : public std::error_category
{
    public:

        /**
         * @brief Returns the singleton instance of the category
        */
        static const DerivedErrorCategoryT& get()
        {
            static DerivedErrorCategoryT c;
            return c;
        }

        /**
         * @brief Deleted to prevent more than once instance.
         */
        Error_Category( const Error_Category& ) = delete;

        /**
         * @brief Deleted to prevent more than once instance.
         */
        Error_Category& operator = ( const Error_Category& ) = delete;

        /**
         * @brief Returns the human-readable name of the category.
         *
         * Returns the human-readable name
        */
        const char* name() const noexcept final
        {
            return name_();
        }

        /**
         * @brief Returns the human readable name of the error code (determined by the impl
         *        of the protected `message_()` function. Which is implemented in the derived
         *        class.
         */
        std::string message( int c ) const final
        {
            return message_(c);
        }

    protected:

        /**
         * Returns the human-readable name of the category.
         */
        virtual const char* name_() const noexcept = 0;

        /**
         * @brief Converts an integer error code into a human-readable name for the code.
         *
         * Names are based on the integer represetation as custom error codes.
        */
        virtual std::string message_( int c ) const = 0;

        /**
         * @brief Private due to being a singleton
         */
        Error_Category() = default;

        /**
         * @brief Private due to being a singleton.
        */
        virtual ~Error_Category() = default;

}; // End of Error_Category Class

} // End of tmns::outcome namespace