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

namespace CalcEval::Type
{
    /** Type::Double struct implementation.

        Inherits the Type::Base and implements it for double type.

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

    };

} // namespace CalcEval::Type

#endif // CALCEVAL_TYPE_DOUBLE_HPP
