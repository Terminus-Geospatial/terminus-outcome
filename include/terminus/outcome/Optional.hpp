/**
 * @file    macros.hpp
 * @author  Marvin Smith
 * @date    7/7/2023
*/
#pragma once


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
        explicit Optional( const std::optional<ValueT> )
        required ( std::array)
}; // End of Optional Class

} // End of tmns::outcome namespace