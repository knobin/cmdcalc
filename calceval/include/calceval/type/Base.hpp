//
//  type/Base.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2021-09-03.
//

#ifndef CALCEVAL_TYPE_BASE_HPP
#define CALCEVAL_TYPE_BASE_HPP

// C++ Headers
#include <functional>
#include <string>
#include <string_view>
#include <optional>

namespace CalcEval::Type
{
    /** Type::Base struct implementation.

        Base class for Type that the parser that contains necessary
        functions that it needs to work.

        The class contains these functions:
            - stot : function to convert a string to the type.
            - constant : function to get math constant value from string.
            - function : function to get math function from string.

        This makes it extensible so constants and functions can be added
        easily by creating a new type and using it in the parser.

        Only member function that has to be implemented is stot, the
        other member functions are optional since no constans or functions
        has to be available in the parser.
    */
    template<typename CalcType>
    struct Base
    {
        using value_type = CalcType;
        using func_type = std::function<value_type(value_type)>;

        /** Default Base constructor

            @return     default initialized Base
        */
        Base() noexcept = default;

        /** Destructor for Base

        */
        virtual ~Base() noexcept = default;

        /** Function for converting a string to the type.

            Implementation of this member function must not ever throw, the string
            to convert must be able to be converted. The string will come from the
            Scanner and that will make sure that it contains a floating point number.

            Depending on the method and type used, this function might have rounding
            issues and other faults with floating numbers in general.
            Something to keep in mind.

            @param  str     string to convert
            @return         value
        */
        [[nodiscard]] virtual value_type stot(const std::string&) noexcept = 0;

        /** Function for retrieving a math constant from certain string.

            It returns the value in an std::optional, if no constant can be found it returns
            an std::nullopt.

            @param  str     name of constant
            @return         constant value as an std::optional
        */
        [[nodiscard]] virtual std::optional<value_type> constant(const std::string&) noexcept
        {
            return std::nullopt;
        }

        /** Function for retrieving a math function from certain string.

            It returns the value in an std::optional, if no math function can be found it
            returns an std::nullopt.

            @param  str     name of function
            @return         math function as an std::optional
        */
        [[nodiscard]] virtual std::optional<func_type> function(const std::string&) noexcept
        {
            return std::nullopt;
        }
    };

} // namespace CalcEval::Type

#endif // CALCEVAL_TYPE_BASE_HPP
