//
//  ParserLogic.tpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2021-08-26.
//

#ifndef CALCEVAL_PARSERLOGIC_TPP
#define CALCEVAL_PARSERLOGIC_TPP

// C++ Headers
#include <cmath>
#include <functional>
#include <utility>

namespace CalcEval
{
    template<typename CalcType>
    ParserLogic<CalcType>::ParserLogic(std::istringstream& iss) : m_scanner{iss}
    {
    }

    template<typename CalcType>
    ParserLogic<CalcType>::ParserLogic(std::ifstream& ifs) : m_scanner{ifs}
    {
    }

    template<typename CalcType>
    void ParserLogic<CalcType>::scan()
    {
        do
        {
            m_token = m_scanner.scan();
        } while (m_token.type == TokenType::EndOfLine);
    }

    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::parse()
    {
        value_type val{0.0};
        scan();

        if (m_token.type != TokenType::Bad)
        {
            val = expr();
        }

        if (m_token.type != TokenType::EndMark)
        {
            error(m_token, "");
        }

        return val;
    }

    // <expr> ::= <term><expr_tail>
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::expr()
    {
        const value_type lhs{term()};
        return exprTail(lhs);
    }

    // <expr_tail> ::= +<term><expr_tail>
    //      |   -<term><expr_tail>
    //      |   <empty>
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::exprTail(value_type lhs)
    {
        value_type val{lhs};

        if (m_token.type == TokenType::Plus)
        {
            scan();
            val += term();
            return exprTail(val);
        }
        else if (m_token.type == TokenType::Minus)
        {
            scan();
            val -= term();
            return exprTail(val);
        }

        return val;
    }

    // <term> ::= <factor><term_tail>
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::term()
    {
        const value_type lhs{factor()};
        return termTail(lhs);
    }

    // <term_tail> ::= *<factor><term_tail>
    //      |   /<factor><term_tail>
    //      |   <empty>
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::termTail(value_type lhs)
    {
        value_type val{lhs};

        if (m_token.type == TokenType::Multiply)
        {
            scan();
            val *= factor();
            return termTail(val);
        }
        else if (m_token.type == TokenType::Divide)
        {
            scan();
            val /= factor();
            return termTail(val);
        }

        return val;
    }

    // <factor> ::= -<value><factor_term>
    //      |   <value><factor_term>
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::factor()
    {
        value_type multi{1};

        if (m_token.type == TokenType::Minus)
        {
            scan();
            multi = value_type{-1};
        }

        const value_type lhs{value()};
        return factorTail(lhs) * multi;
    }

    // <factor_term> ::= ^-<value><factor_tail>
    //      |   ^<value><factor_tail>
    //      |   <empty>
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::factorTail(value_type lhs)
    {
        value_type val{lhs};

        if (m_token.type == TokenType::Power)
        {
            scan();
            value_type multi{1};
            if (m_token.type == TokenType::Minus)
            {
                scan();
                multi = value_type{-1};
            }

            val = pow(val, factorTail(value()) * multi);
        }

        return val;
    }

    // <value> ::= ( <expr> )
    //      |   <id>
    //      | 	num
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::value()
    {
        value_type val{0};

        if (m_token.type == TokenType::LeftParen)
        {
            scan();
            val = expr();
            if (m_token.type != TokenType::RightParen)
            {
                error(m_token, "')'");
            }
        }
        else if (m_token.type == TokenType::Identifier)
        {
            return id();
        }
        else if (m_token.type == TokenType::Number)
        {
            //val = CalcType{m_token.value};
            val = m_calcType.stot(m_token.value);
        }
        else
        {
            error(m_token, "'(', identifier or number");
        }

        scan();
        return val;
    }

    // <id> ::= function <value>
    //      |   constant
    template<typename CalcType>
    typename ParserLogic<CalcType>::value_type ParserLogic<CalcType>::id()
    {
        if (m_token.type == TokenType::Identifier)
        {
            const Token token{m_token};
            const std::string& str{token.value};
            scan();

            // Expect a constant
            if (m_token.type != TokenType::LeftParen)
            {
                if (auto constant = m_calcType.constant(str))
                {
                    return *constant;
                }
                else if (auto func = m_calcType.function(str))
                {
                    error(token, "constant, no such constant found\nDid you mean to call " + str + "(x)?");
                }
                else
                {
                    error(token, "constant, no such constant found");
                }
            }

            // Is str a function?
            if (auto func = m_calcType.function(str))
            {
                if (m_token.type == TokenType::LeftParen)
                {
                    // Should be a valid function here.
                    // value() might fail, but that is a valid error.
                     return (*func)(value());
                }

                // it is a function but missing its starting parentheses.
                error(m_token, "'(', function found");
            }
            else if (m_token.type == TokenType::LeftParen)
            {
                // No function found, but has '(', so a function is expected.
                error(token, "function, no such function found");
            }

            // Neither error or function.
            // This line should in theory never be reached.
            error(token, "constant or function, no such constant or function found");
        }


        // Neither error or function, since '(' is not found we expect a constant.
        error(m_token, "identifer");
        return value_type{0.0};
    }

    template<typename CalcType>
    void ParserLogic<CalcType>::error(const Token& token, const std::string& expected) const
    {
        const std::string unexpectedMsg{"token of \"" + std::string{tokenStr(token.type)} + "\""};
        const std::string expectedMsg{(!expected.empty()) ? "Expected " + expected + "!" : ""};

        const std::string content{m_scanner.scanned()};
        std::size_t start{0};
        const std::size_t findLast{content.find_last_of('\n')};
        if (findLast != std::string::npos)
            start = findLast + 1;

        const std::string line{content.substr(start)};
        throw ParserError(line, token.location, unexpectedMsg, expectedMsg, token);
    }

} // namespace CalcEval

#endif // CALCEVAL_PARSERLOGIC_TPP
