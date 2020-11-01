//
//  Parser.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

#ifndef CALCEVAL_PARSER_HPP
#define CALCEVAL_PARSER_HPP

// Local Headers
#include "calceval/Scanner.hpp"

namespace CalcEval
{
    class Parser
    {
    public:
        Parser() = delete;
        explicit Parser(std::istream& is) : m_scanner{is} {}

        double parse();

    private:
        void scan();
        double expr();
        double exprTail(double lhs);
        double term();
        double termTail(double lhs);
        double factor();
        double unary();
        double value();
        double id();

    private:
        Scanner m_scanner;
        Token m_token{TokenType::EndMark, {0,0}, ""};
    };

    class ParserError : public std::logic_error
    {
    public:
        using std::logic_error::logic_error;
    };

} // namespace CalcEval

#endif // CALCEVAL_PARSER_HPP
