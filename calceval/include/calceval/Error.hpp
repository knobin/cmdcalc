//
//  Error.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-11-04.
//

#ifndef CALCEVAL_ERROR_HPP
#define CALCEVAL_ERROR_HPP

// Local Headers
#include "calceval/Token.hpp"

// C++ Headers
#include <stdexcept>
#include <string>

namespace CalcEval
{
    class Error : public std::logic_error
    {
    public:
        using std::logic_error::logic_error;

        Error(const std::string& line, Location location, const std::string& unexpected,
              const std::string& expected = "");

        [[nodiscard]] const std::string& line() const noexcept;

        [[nodiscard]] Location location() const noexcept;

    private:
        std::string m_line;
        Location m_loc;
    };

} // namespace CalcEval

#endif // CALCEVAL_ERROR_HPP
