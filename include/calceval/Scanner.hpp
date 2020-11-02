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
        explicit Scanner(std::istream& is) : m_stream{is} {}

        [[nodiscard]] Token scan();

    private:
        bool ignoreWhitespaces();
        std::string readIdentifier();
        std::string readDigit();
        std::pair<char, TokenType> readSymbol();

    private:
        std::istream& m_stream;
        Location m_cLoc{1, 1};
        int m_next{0};
    };

    class ScannerError : public std::logic_error
    {
    public:
        using std::logic_error::logic_error;
    };

} // namespace CalcEval

#endif // CALCEVAL_SCANNER_HPP
