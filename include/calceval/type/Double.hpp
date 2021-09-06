//
//  type/Double.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2021-09-03.
//

#ifndef CALCEVAL_TYPE_DOUBLE_HPP
#define CALCEVAL_TYPE_DOUBLE_HPP

// Local Headers
#include "Base.hpp"

// C++ Headers
#include <cmath>
#include <map>

namespace CalcEval::Type
{
    /** Type::Double struct implementation.

        Inherits the Type::Base and implements it for double type with
        2 constant values and 9 functions.

        Included constants:
            - pi : 3.14159265
            - e  : 2.71828183

        Included functions:
            - log
            - log10
            - exp
            - sin
            - cos
            - tan
            - arcsin
            - arccos
            - arctan

        stot member function is implemented with std::stod.
    */
    struct Double : public Base<double>
    {
        /** Function for converting a string to double.

            Implemented with std::stod.

            @param  str     string to convert
            @return         value
        */
        [[nodiscard]] value_type stot(const std::string& str) noexcept override
        {
            return std::stod(str);
        }

        /** Function for retrieving a math constant from certain string.

            Included constants:
                - pi : 3.14159265
                - e  : 2.71828183

            Otherwise returns std::nullopt.

            @param  str     name of constant
            @return         constant value as an std::optional
        */
        [[nodiscard]] std::optional<double> constant(const std::string& str) noexcept override
        {
            if (str == "pi")
                return 3.14159265;
            else if (str == "e")
                return 2.71828183;

            return std::nullopt;
        }

        /** Function for retrieving a math function from certain string.

            Included functions:
                - log
                - log10
                - exp
                - sin
                - cos
                - tan
                - arcsin
                - arccos
                - arctan

            Otherwise returns std::nullopt.

            @param  str     name of function
            @return         math function as an std::optional
        */
        [[nodiscard]] std::optional<Base<double>::func_type> function(const std::string& str) noexcept override
        {
            const std::map<std::string_view, Base<double>::func_type> funcs
            {
                std::pair{"log", [](double x) { return std::log(x); }},
                std::pair{"log10", [](double x) { return std::log10(x); }},
                std::pair{"exp", [](double x) { return std::exp(x); }},
                std::pair{"sin", [](double x) { return std::sin(x); }},
                std::pair{"cos", [](double x) { return std::cos(x); }},
                std::pair{"tan", [](double x) { return std::tan(x); }},
                std::pair{"arcsin", [](double x) { return std::asin(x); }},
                std::pair{"arccos", [](double x) { return std::acos(x); }},
                std::pair{"arctan", [](double x) { return std::atan(x); }}
            };

            const auto search = funcs.find(str);

            if (search != funcs.cend())
                return search->second;

            return std::nullopt;
        }
    };

} // namespace CalcEval::Type

#endif // CALCEVAL_TYPE_DOUBLE_HPP
