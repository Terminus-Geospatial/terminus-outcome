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
 * @file    optional.hpp
 * @author  Marvin Smith
 * @date    7/7/2023
*/
#pragma once

// C++ Standard Libraries
#include <concepts>
#include <optional>
#include <type_traits>
#include <utility>

// Boost Libraries
#include <boost/optional.hpp>

namespace tmns::outcome {

/**
 * @brief Wrapper around `boost::optional` that can interoperate with `std::optional`.
 *
 * This class inherits all constructors and behaviors from `boost::optional` while
 * adding bridge constructors, assignment operators, and conversion helpers that
 * make it seamless to exchange state with `std::optional`.
 *
 * @tparam ValueT Value type stored by the optional.
 */
template <class ValueT>
class Optional : public boost::optional<ValueT>
{
    public:

        /// Type alias capturing the value type
        using ValueType = ValueT;

        /// Type alias capturing the decayed value type (used in `std::optional` conversions)
        using DecayedValueType = std::decay_t<ValueT>;

        // Inherit all boost optional constructors
        using boost::optional<ValueT>::optional;

        /**
         * @brief Copy-converting constructor that initializes from a `std::optional`.
         *
         * @param stdopt Source optional whose contained value is copied when present.
        */
        explicit Optional( const std::optional<ValueT>& stdopt )
        requires ( std::copy_constructible<ValueT> && !std::is_reference_v<ValueT> )
        {
            if( stdopt.has_value() )
            {
                this->emplace( *stdopt );
            }
            else
            {
                this->reset();
            }
        }

        /**
         * @brief Move-converting constructor that transfers state from a `std::optional` rvalue.
         *
         * The source optional is cleared when it contains a value.
         *
         * @note Enabled only when `ValueT` is move-constructible and not a reference.
         */
        explicit Optional( std::optional<ValueT>&& stdopt )
        requires( std::move_constructible<ValueT> && !std::is_reference_v<ValueT> )
        {
            if( stdopt.has_value() )
            {
                this->emplace( std::move( *stdopt ) );
            }
            else
            {
                this->reset();
            }
       }

        /**
         * @brief Copy assignment operator that synchronizes with a `std::optional`.
         *
         * @param stdopt Source optional whose value is copied when present.
         * @return Reference to `*this`.
         */
        Optional& operator = ( const std::optional<ValueT>& stdopt )
        requires ( std::copy_constructible<ValueT> && !std::is_reference_v<ValueT> )
        {
            if( stdopt.has_value() )
            {
                this->emplace( *stdopt );
            }
            else
            {
                this->reset();
            }
            return *this;
        }

        /**
         * @brief Move assignment operator that synchronizes with a `std::optional` rvalue.
         *
         * @param stdopt Source optional whose value is moved when present.
         * @return Reference to `*this`.
         */
        Optional& operator = ( std::optional<ValueT>&& stdopt )
        requires ( std::move_constructible<ValueT> && !std::is_reference_v<ValueT> )
        {
            if( stdopt.has_value() )
            {
                this->emplace( std::move( *stdopt ) );
            }
            else
            {
                this->reset();
            }
            return *this;
        }

        /**
         * @brief Copy-conversion operator that produces a `std::optional` view of the value.
         *
         * @note Enabled only when the decayed type of `ValueT` is copy-constructible.
         */
        operator std::optional<DecayedValueType>() const&
        requires std::copy_constructible<DecayedValueType>
        {
            if( this->has_value() )
            {
                return std::optional<DecayedValueType>{ **this };
            }
            else
            {
                return std::nullopt;
            }
        }

        /**
         * @brief Move-conversion operator that produces a `std::optional` view of the value.
         *
         * @return A `std::optional` instance containing the value when present, or `std::nullopt` otherwise.
         *
         * @details This conversion operator is enabled only when the decayed type of `ValueT` is move-constructible.
         */
        operator std::optional<DecayedValueType>() &&
            requires std::move_constructible<DecayedValueType>
        {
            if( this->has_value() )
            {
                return std::optional<DecayedValueType>{ std::move( **this ) };
            }
            else
            {
                return std::nullopt;
            }
        }

}; // End of Optional Class

/// Type tag to help construct an empty `Optional` instance.
using Empty = boost::none_t;

/// Value used to tag-construct an empty `Optional` instance.
constexpr Empty cEmpty = boost::none;

} // End of tmns::outcome namespace