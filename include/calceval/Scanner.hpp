//
//  Scanner.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

#ifndef CALCEVAL_SCANNER_HPP
#define CALCEVAL_SCANNER_HPP

// Local Headers
#include "calceval/Token.hpp"

// C++ Headers
#include <istream>

namespace CalcEval
{
    class Scanner
    {
    public:
        Scanner() = delete;
        explicit Scanner(std::istream& is) : m_stream{is}
        {
        }

        [[nodiscard]] Token scan();

        [[nodiscard]] std::string currentLine() const;

    private:
        bool ignoreWhitespaces();
        std::string readIdentifier();
        std::string readDigit();
        std::pair<char, TokenType> readSymbol();
        void error(const std::string& unexpected, const Location& location) const;

    private:
        std::istream& m_stream;
        Location m_cLoc{1, 1};
        int m_next{0};
    };

    std::string errorMsg(const std::string& unexpected, const std::string& line,
                         const Location& location);
    std::string errorFromLine(const std::string& line, std::size_t at);

    class ScannerError : public std::logic_error
    {
    public:
        using std::logic_error::logic_error;
    };

} // namespace CalcEval

#endif // CALCEVAL_SCANNER_HPP
