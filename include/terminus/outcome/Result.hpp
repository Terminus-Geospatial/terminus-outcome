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
 * @file    macros.hpp
 * @author  Marvin Smith
 * @date    7/7/2023
*/
#pragma once

// Project Libraries
#include "Error.hpp"

// Boost Libraries
#include <boost/outcome/result.hpp>
#include <boost/outcome/utils.hpp>

// C++ Libraries
#include <future>
#include <system_error>
#include <type_traits>

// Namespace alias for the boost outcome namespace
namespace tmns::outcome::impl {
    namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;
} // end of tmns::outcome::impl namespace

namespace tmns::outcome {

/**
 * Contains the result of an operation that could potentially fail, offering an alternative to
 * error handling as opposed to exceptions.
 *
 * The type is built on top of Boost.Outcome `basic_result` type.  Its API is identical to
 * that type with one additional enhancement: our `Result` type supports references.
 *
 * This result type fits into the standard library's `std::error_code` error handling framework
 * by using the `Error` type as the failure outcome type for the result.
 *
 * @see https://www.boost.org/doc/libs/1_81_0/libs/outcome/doc/html/index.html
*/
template <class ValueT, class ErrorCodeT>
class Result;

/**
 * Specialization of the `Result` type for non-reference types.  This is basically just a
 * pass-through to the underlying Boost `result` type.
*/
template <typename  ValueT, typename ErrorCodeT>
  requires( !std::is_lvalue_reference_v<ValueT> )
class Result<ValueT,ErrorCodeT> : public impl::outcome::result<ValueT,Error<ErrorCodeT>>
{
    public:
        using impl::outcome::result<ValueT, Error<ErrorCodeT>>::result;
}; // End of Result Class

/**
 * Specialization of the `Result` type for reference types.  This effectively wraps the non-ref
 * type of `ValueT` inside of a std::reference_wrapper` and then overloads a few of the public
 * Boost.Outcome `result` methods to hide the fact that the value is stored inside a wrapper type.
 *
 * Note that we don't have to account for the case when `ValueT` is `void` because we can't do
 * anything with a `void&` type anyways.
*/
template <class ValueT, class ErrorCodeT> requires ( std::is_lvalue_reference_v<ValueT> )
class Result<ValueT,ErrorCodeT>
     : public impl::outcome::result<std::reference_wrapper<std::remove_reference_t<ValueT>>,
                                    Error<ErrorCodeT>>
{
    private:

        using BASE_TYPE = impl::outcome::result<std::reference_wrapper<std::remove_reference_t<ValueT>>,
                                                Error<ErrorCodeT>>;

    public:

        // Expose the inherited constructors
        using impl::outcome::result<std::reference_wrapper<std::remove_reference_t<ValueT>>,
                                    Error<ErrorCodeT>>::result;

        /**
         * Override the base methods for accessing the value so that we can get the value inside the
         * reference wrapper without making the user call `get()` all over the place.
        */
        ValueT& assume_value() &
        {
            return BASE_TYPE::assume_value().get();
        }

        const ValueT& assume_value() const&
        {
            return BASE_TYPE::assume_value().get();
        }

        ValueT&& assume_value() &&
        {
            return BASE_TYPE::assume_value().get();
        }

        const ValueT&& assume_value() const&&
        {
            return BASE_TYPE::value().get();
        }

        ValueT& value() &
        {
            return BASE_TYPE::value().get();
        }

        const ValueT& value() const&
        {
            return BASE_TYPE::value().get();
        }

        ValueT&& value() &&
        {
            return BASE_TYPE::value().get();
        }

        const ValueT&& value() const&&
        {
            return BASE_TYPE::value().get();
        }

}; // End of Result Class


/**
 * A result wrapped inside of a future object.
 *
 * Futures are used in asynchronous operations.  In other contexts, when an asynchronous
 * operation fails, an exception is usually set so that attempting to access the future value
 * results in a throw.  Using the semantics of this library however, calling `get()` on a
 * `FutureResult` should never result in an exception being thrown.  Instead, the caller should
 * immediately check the return value of `get()` to inspect whether or not the result contains a
 * success value or a failure value.
*/
template <class ValueT, class ErrorCodeT>
using FutureResult = std::future<Result<ValueT,ErrorCodeT>>;

/**
 * Overload that tells the Boost Outcome library how to transform our custom error with a payload
 * into a `std::system_error` and throw it.  The library needs this in order to successfully use our
 * custom error type.
 *
 * @note Calling this function will always throw an exception.
 *
 * @param e The error containing information used to construct a system error and throw it.
*/
template <class ErrorCodeT>
inline void outcome_throw_as_system_error_with_payload( Error<ErrorCodeT> e )
{
    impl::outcome::try_throw_std_exception_from_error( e.code() );
    throw std::system_error( e.code(), e.details() );
}

/**
 * Utility function that creates a successful outcome result by constructing the type in place,
 * forwarding the provided arguments onto the underlying success type's constructor.
 *
 * @param args The constructor arguments to forward to the success type's constructor.
 *
 * @return A newly constructed result type in the success state.
*/
template <class ValueT, class... ArgsT>
auto ok( ArgsT&&... args )
{
    ValueT v( std::forward<ArgsT>( args )... );
    return impl::outcome::success( std::move( v ) );
}

/**
 * Utility function that specializes creating a successful result type when the success type is `void`.
 *
 * @return A newly constructed result type in the success state.
*/
inline auto ok()
{
    return impl::outcome::success();
}

/**
 * Utility function that helps create a failure outcome result by constructing the failure type
 * in-place, forwarding the provided arguments onto the underlying failure type's constructor.
 *
 * In this case, our failure type is always `Error`, so we don't need an additional template
 * parameter to indicate what failure type to construct.
 *
 * @note  This does not actually construct the result type.  Instead, it constructs a tagged failure
 * type that is used by the `Result` constructor to create a result in the failure state.  This is
 * done in order to avoid having to pass the success type as a template parameter in order to use
 * `Result<ValueT>` as the return type.  This extra template parameter would make function usage
 * confusing because it would require specifying a type that doesn't have anything to do with
 * the error being constructed.
 *
 * @param args The arguments to forward to the failure type's constructor.
 *
 * @returns A tagged type used to construct a result in the failure state.
*/
template <class ErrorCodeT, class... ArgsT>
auto fail( ErrorCodeT c, ArgsT&&... args )
{
    return impl::outcome::failure( Error<ErrorCodeT>{ c, std::forward<ArgsT>( args )...} );
}

/**
 * Utility function that helps create a failure result from a copy or move of a previous error.
 * This is especially useful when using the `Error::append()` function to add information to the
 * error message as it propogates back up the call stack.
 *
 * @param e The original `Error` instance.
 *
 * @returns A tagged type used to construct a result in the failure state.
*/
template <class ErrorT>
auto fail( ErrorT&& e )
{
    return impl::outcome::failure( std::forward<ErrorT>( e ) );
}

/**
 * Utility function that constructs a new Error and throws it as a `std::system_error`.
 *
 * Use this function to indicate behavior that should result in the termination of the program.
 * Rather than call `std::abort()` or a similar function here, we throw an exceeption so that the
 * program can unwind the stack and deallocate memory where needed.
 *
 * This utility function is provided in addition to the no-value policies provided by the Boost
 * Outcome library.
 *
 * @param args The arguments used ot construct an instance of `Error`.
*/
template <class ErrorCodeT, class... ArgsT>
void panic( ErrorCodeT c, ArgsT&&... args )
{
    Error e( c, std::forward<ArgsT>( args )... );
    outcome_throw_as_system_error_with_payload( std::move( e ) );
}

} // End of tmns::outcome namespace