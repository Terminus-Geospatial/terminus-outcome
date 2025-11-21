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

// C++ Includes
#include <system_error>
#include <type_traits>

/**
 * Helper macro for concatenating two symbol names together
*/
#define __TERMINUS_OUTCOME_IMPL_CONCAT_IMPL(x, y) x##y

/**
 * Helper macro used for double-indirection when concatenating two symbol names together. If `x`
 * or `y` are preprocessor macro definitions (e.g. `__LINE__`), then they will be replaced here
 * before being forwarded on to the actual concatenation macro.
*/
#define __TERMINUS_OUTCOME_IMPL_CONCAT(x, y) __TERMINUS_OUTCOME_IMPL_CONCAT_IMPL(x, y)

/**
 * Helper macro for generating a unique name for the intermediate variable that holds the
 * result of an expression in the `TERMINUS_OUTCOME_TRY` macro.  The macro effectively appends the
 * current line number to the provided variable name to generate a unique name.
*/
#define __TERMINUS_OUTCOME_IMPL_RES_VAR __TERMINUS_OUTCOME_IMPL_CONCAT(__result, __LINE__)

/**
 * Helper macro that enabled C-style macro overloading.  The macro expands into a macro called
 * `NAME` based on the number of parameters supplied to the macro invoking this one.
 *
 * @see TERMINUS_OUTCOME_TRY
*/
#define __TERMINUS_OUTCOME_TRY_SELECT_OVERLOAD(_1, _2, NAME, ... ) NAME

/**
 * Helper macro that evaluates an expression returning a `Result` type and returns early from
 * a function if the result contains an error.  The early return will return the contained error.
 *
 * This macro is safe to use multiple times in the same function as long as each use is on a
 * different line.  The intermediate variable name that holds the result value is generated using
 * the line number.
*/
#define __TERMINUS_OUTCOME_TRY_1(expression)                             \
    auto __TERMINUS_OUTCOME_IMPL_RES_VAR = expression;                   \
    if(__TERMINUS_OUTCOME_IMPL_RES_VAR.has_error())                      \
    {                                                                    \
        return std::move(__TERMINUS_OUTCOME_IMPL_RES_VAR).as_failure() ; \
    }

/**
 * Helper macro that, after evaluating an expression returning a `Result` type, will assign the
 * success value of the result to the variable `var`.  If, after evaluating the expression, the
 * result contains an error, then the macro causes the function to return early.  The early return
 * will return the contained error.
 *
 * This macro is safe to use multiple times in the same function as long as each use is on a
 * different line.  The intermediate variable name that holds the result value is generated using
 * the line number.
*/
#define __TERMINUS_OUTCOME_TRY_2(var, expression)    \
    __TERMINUS_OUTCOME_TRY_1(expression)             \
    var = std::move(__TERMINUS_OUTCOME_IMPL_RES_VAR).assume_value();

/**
 * Generates the boilerplace code that normally checks the result of an expression that returns a
 * `Result` type and returns early from the function if the result contains an error.
 *
 * There are two ways you can use this macro:
 *
 * 1. `TERMINUS_OUTCOME_TRY( expression )`: Given an expression that evaluates to a `Result` type, check
 *      to see if the result contains an error.  If it does, return early from the function.  The name
 *      of the intermediate variable used to capture the result of evaluating `expression` should
 *      not be accessed directly.  If you need to access the contained "success" value of the result
 *      in the event that `expression` succeeds, see the second way to use this macro.
 *
 * 2. `TERMINUS_OUTCOME_TRY( var, expression )`: Given variable name/declaration and an expression that
 *      evaluates to a `Result` type, check to see if the result contains an error.  If it does,
 *      return early from the function.  If it doesn't, (i.e. the operation succeeds), "steal" the
 *      success value of the operation and assign it to the variable named `var`.  This uses move
 *      semantics to transfer ownership of the success value from the `Result` type to `var`
 *      efficiently.
 *
 * @note This macro uses C-style macro overloading, which makes it possible to use the macro in the
 *       ways described above.
 *
 * @note  The `static_assert(true)` as the end of the macro (without a semi-colon at the end) is
 *        needed in order to supress warnings from SonarQube about empty statements.  Unfortunetly,
 *        `clang-format` expects there to be a semi-colon after a macro or else it indents it, which is
 *        not desirable in the formatter.  The easiest way to fix this is to just add empty static asserts
 *        at the end of our macros so we can add the semi-colon and make both `clang-format` and SonarQube
 *        happy.
*/
#define TERMINUS_OUTCOME_TRY(...)                                                                           \
    __TERMINUS_OUTCOME_TRY_SELECT_OVERLOAD(__VA_ARGS__, __TERMINUS_OUTCOME_TRY_2, __TERMINUS_OUTCOME_TRY_1) \
    (__VA_ARGS__) static_assert( true )

/**
 * Generates the boilerplate code required to connect a set of enumerated error codes of type
 * `ErrorCodeT` with the error category that can properly interpret them.
 *
 * @note This macro can be used inside of a namespace.  If it is not used inside of the same
 *       namespace as the `ErrorCategoryT` and `ErrorCodeT` types, then the values for those parameters
 *       should be fully-qualified.  The recommended approach is to use the macro i the same namespace
 *       as those types and take advantage of argument-dependent-lookup (ADL) to properly find the
 *       definition.
 *
 * @param ErrorCategoryT the error category type (should be derived type provided by the user).
 * @param ErrorCodeT    The type of the enum class that contains the set of error code variants.
*/
#define TERMINUS_OUTCOME_ASSOCIATE_ERROR_CATEGORY( ErrorCategoryT, ErrorCodeT ) \
    inline std::error_code make_error_code( ErrorCodeT e ) {    \
        return {static_cast<int>(e), ErrorCategoryT::get() };   \
    }

/**
 * Generates the boilerplate code required to tag a custom error code enumeration type so
 * that the newer standard library `error_code` framework can pick up on the type.
 *
 * @note This macro should not be used inside of a namespace!
 *
 * @param ErrorCodeT The enum class type that contains the set of error code variants to use.
*/
#define TERMINUS_OUTCOME_MARK_ENUM(ErrorCodeT)                  \
    namespace std {                                             \
    template <>                                                 \
    struct is_error_code_enum<ErrorCodeT> : std::true_type {};  \
    }
