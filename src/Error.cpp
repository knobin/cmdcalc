//
//  Error.cpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-11-04.
//

// Local Headers
#include "calceval/Error.hpp"

// C++ Headers
#include <sstream>

namespace CalcEval
{
    Error::Error(const std::string& line, Location location, const std::string& unexpected,
                 const std::string& expected)
        : std::logic_error(errorMsg(unexpected, line, location, expected)), m_line{line},
          m_loc{location}
    {
    }

    const std::string& Error::line() const noexcept
    {
        return m_line;
    }

    Location Error::location() const noexcept
    {
        return m_loc;
    }

    ///////////////////////////////////////////////////////////////////////////////

    std::string errorLineMsg(const std::string& line, Location::value_type at)
    {
        std::ostringstream oss{};
        oss << line << '\n' << ((at > 1) ? std::string(static_cast<std::size_t>(at - 1), '-') : "") << '^';
        return oss.str();
    }

    std::string errorMsg(const std::string& unexpected, const std::string& line,
                         const Location& location, const std::string& expected)
    {
        std::ostringstream oss{};
        oss << "Unexpected " << unexpected << " at line " << location.line << " : "
            << location.column << "\n"
            << errorLineMsg(line, location.column) << ((!expected.empty()) ? "\n" + expected : "");

        return oss.str();
    }

} // namespace CalcEval
