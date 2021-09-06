//
//  ParserLogic.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2021-08-28.
//

#ifndef CALCEVAL_PARSERLOGIC_HPP
#define CALCEVAL_PARSERLOGIC_HPP

// Local Headers
#include "calceval/Error.hpp"
#include "calceval/Scanner.hpp"

// C++ Headers
#include <fstream>
#include <sstream>

namespace CalcEval
{
    /** ParserLogic class implementation.

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

        <factor> ::= -<value><factor_tail>
	    |   <value><factor_tail>

        <factor_tail> ::= ^-<value><factor_tail>
            |   ^<value><factor_tail>
            |   <empty>

        <value> ::= ( <expr> )
            |   <id>
            |   num

        <id> ::= function <value>
            |   constant
    */
    template<typename CalcType>
    class ParserLogic
    {
    public:
        using value_type = typename CalcType::value_type;

    public:
        /** Default ParserLogic constructor is disabled.
         *
            For now, it must be initialized with an istream.
            Might change in the future though.
        */
        ParserLogic() = delete;

        /** ParserLogic constructor with iss.

            @param  iss     istringstream to use
            @return         default initialized ParserLogic
        */
        explicit ParserLogic(std::istringstream& iss);

        /** ParserLogic constructor with ifs.

            @param  ifs     ifstream to use
            @return         default initialized ParserLogic
        */
        explicit ParserLogic(std::ifstream& ifs);

        /** Function for parsing the istream in the scanner.

            A limitation is that the ParserLogic is limited to parse
            a double and use a double. This can be improved in
            the future though.

            @return     resulting value
        */
        [[nodiscard]] value_type parse();

    private:
        /** Function for scanning.

            It skips TokenType::EndOfLine and then sets the
            m_token member value.
        */
        void scan();

        /** Function starting the expr.

            @return     resulting value
        */
        value_type expr();

        /** Function for the expr_tail.

            @param  lhs     lhs of the expr
            @return         resulting value
        */
        value_type exprTail(value_type lhs);

        /** Function for the term.

            @return     resulting value
        */
        value_type term();

        /** Function for the term_tail.

            @param  lhs     lhs of the term
            @return         resulting value
        */
        value_type termTail(value_type lhs);

        /** Function for the factor.

            @return     resulting value
        */
        value_type factor();

        /** Function for the factor_tail.

            @return     resulting value
        */
        value_type factorTail(value_type lhs);

        /** Function for the value.

            @return     resulting value
        */
        value_type value();

        /** Function for the id.

            @return     resulting value
        */
        value_type id();

        /** Function to throw an error.

            @param  token           token that caused the error
            @param  unexpected      what was unexpected
        */
        void error(const Token& token, const std::string& expected) const;

    private:
        Scanner m_scanner;
        Token m_token{TokenType::EndMark, {0, 0}, ""};
        CalcType m_calcType{};
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

// Implementation of ParserLogic is in a separate file.
#include "ParserLogic.tpp"

#endif // CALCEVAL_PARSERLOGIC_HPP
