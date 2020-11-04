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
        Error(const std::string& line, Location location, const std::string& unexpected,
              const std::string& expected = "");

        [[nodiscard]] const std::string& line() const noexcept;

        [[nodiscard]] Location location() const noexcept;

    private:
        std::string m_line;
        Location m_loc;
    };

    /** Function to construct an error message based on the line.

        @param  line    the line that it was encountered on
        @param  at      the location it was encountered on
        @return         error message
    */
    std::string errorLineMsg(const std::string& line, std::size_t at);

    /** Function to construct an error message.

       @param  unexpected      what was unexpected
       @param  line            the line that it was encountered on
       @param  location        the location it was encountered on
       @return                 error message
   */
    std::string errorMsg(const std::string& unexpected, const std::string& line,
                         const Location& location, const std::string& expected);

} // namespace CalcEval

#endif // CALCEVAL_ERROR_HPP
