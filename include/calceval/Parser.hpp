//
//  Parser.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

#ifndef CALCEVAL_PARSER_HPP
#define CALCEVAL_PARSER_HPP

// Local Headers
#include "calceval/Error.hpp"
#include "calceval/Scanner.hpp"

namespace CalcEval
{
    /** Parser class implementation.

        Object that implements the logic for the calculator syntax.
        Throws ParserError when an error is encountered.

        Grammar is:
        <expr> ::= <term><expr_tail>

        <expr_tail> ::= +<term><expr_tail>
            |   -<term><expr_tail>
            |   <empty>

        <term> ::= <factor><term_tail>

        <term_tail> ::= *<factor><term_tail>
            |   /<factor><term_tail>
            |   <empty>

        <factor> ::= <unary> ^ <unary>
                |	<unary>

        <unary> ::= -<value>
                |	<value>

        <value> ::= ( <expr> )
                | 	<id>
                | 	num

        <id> ::= function <value>
                | 	constant
    */
    class Parser
    {
    public:
        /** Default Parser constructor is disabled.

            For now, it must be initialized with an istream.
            Might change in the future though.
        */
        Parser() = delete;

        /** Default Parser constructor with is.

            If std::cin is passed it will cause issues, for details
            look in the Scanner.hpp file.

            @param  is      stream to scan
            @return         default initialized Parser
        */
        explicit Parser(std::istream& is) : m_scanner{is}
        {
        }

        /** Function for parsing the istream in the scanner.

            A limitation is that the parser is limited to parse
            a double and use a double. This can be improved in
            the future though.

            @return     resulting value
        */
        double parse();

    private:
        /** Function for scanning.

            It skips TokenType::EndOfLine and then sets the
            m_token member value.
        */
        void scan();

        /** Function starting the expr.

            @return     resulting value
        */
        double expr();

        /** Function for the expr_tail.

            @param  lhs     lhs of the expr
            @return         resulting value
        */
        double exprTail(double lhs);

        /** Function for the term.

            @return     resulting value
        */
        double term();

        /** Function for the term_tail.

            @param  lhs     lhs of the term
            @return         resulting value
        */
        double termTail(double lhs);

        /** Function for the term_tail.

            @return     resulting value
        */
        double factor();

        /** Function for the unary.

            @return     resulting value
        */
        double unary();

        /** Function for the value.

            @return     resulting value
        */
        double value();

        /** Function for the id.

            @return     resulting value
        */
        double id();

        /** Function to throw an error.

            @param  token           token that caused the error
            @param  unexpected      what was unexpected
        */
        void error(const Token& token, const std::string& expected) const;

    private:
        Scanner m_scanner;
        Token m_token{TokenType::EndMark, {0, 0}, ""};
    };

    /** ParserError class implementation.

        Object that is thrown when the Parser encounters an error.
        Use the ParserError.what() function for details.
    */
    class ParserError : public Error
    {
    public:
        ParserError(const std::string& line, Location location, const std::string& unexpected,
                    const std::string& expected, const Token& token)
            : Error(line, location, unexpected, expected), m_token{token}
        {
        }

        [[nodiscard]] Token token() const noexcept
        {
            return m_token;
        }

    private:
        Token m_token;
    };

} // namespace CalcEval

#endif // CALCEVAL_PARSER_HPP
