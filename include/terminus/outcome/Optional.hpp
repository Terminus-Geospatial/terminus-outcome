/**
 * @file    macros.hpp
 * @author  Marvin Smith
 * @date    7/7/2023
*/
#pragma once

// Boost Libraries
#include <boost/optional.hpp>

// C++ Libraries
#include <concepts>
#include <optional>
#include <type_traits>

namespace tmns::outcome {

template <class ValueT>
class Optional : public boost::optional<ValueT>
{
    public:

        /// Type alias capturing the value type
        using ValueType = ValueT;

        /// Type alias capturing the decayed value type
        // (used in `std::optional` conversions)
        using DecayedValueType = std::decay_t<ValueT>;

        // Inherit all boost optional constructors
        using boost::optional<ValueT>::optional;

        /**
         * Copy-converting constructor that allows us to create our
         * optonal type from a `std::optional` constructor
        */
        explicit Optional( const std::optional<ValueT>& stdopt )
        requires ( std::copy_constructible<ValueT> && !std::is_reference_v<ValueT> )
        {
            if( stdopt.has_value() )
            {
                this->emplace( *stdopt );
            }
        }

        /**
         * Move-converting constructor that allows us to create our optional type from a
         * `std::optional` by "stealing" the optional type's value if possible.
         *
         * @note  This function is only enabled if `ValueT` is move-constructable and is not a
         *        reference
         */
        explicit Optional( std::optional<ValueT>&& stdopt )
        requires( std::move_constructible<ValueT> && !std::is_reference_v<ValueT> )
        {
            if( stdopt.has_value() )
            {
                this->emplace( std::move( *stdopt ) );
            }
       }

        /**
         * Copy-conversion operator that will transform our type, which derives from `boost::optional`,
         * into a `std::optional` to facilitate usage in other libraries that may rely on the
         * `std::optional` type.
         *
         * @note  This function is only enabled if the decayed type of `ValueT` is copy-constructible.
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
         * Move-conversion operator that will transform our type, which derives from `boost::optional`,
         * into a `std::optional` to facilitate usage in other libraries that may rely on the
         * `std::optional` type.
         *
         * @note  This function is only enabled if the decayed type of `ValueT` is move-constructible.
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