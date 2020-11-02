//
//  Parser.cpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

// Local Headers
#include "calceval/Parser.hpp"

// C++ Headers
#include <array>
#include <cmath>
#include <functional>
#include <utility>

namespace CalcEval
{
    void Parser::scan()
    {
        do
        {
            m_token = m_scanner.scan();
        } while (m_token.type == TokenType::EndOfLine);
    }

    double Parser::parse()
    {
        double val{0.0};
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
    double Parser::expr()
    {
        const double lhs{term()};
        return exprTail(lhs);
    }

    // <expr_tail> ::= +<term><expr_tail>
    //      |   -<term><expr_tail>
    //      |   <empty>
    double Parser::exprTail(double lhs)
    {
        double val{lhs};

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
    double Parser::term()
    {
        const double lhs{factor()};
        return termTail(lhs);
    }

    // <term_tail> ::= *<factor><term_tail>
    //      |   /<factor><term_tail>
    //      |   <empty>
    double Parser::termTail(double lhs)
    {
        double val{lhs};

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

    // <factor> ::= <unary> ^ <unary>
    //      |	<unary>
    double Parser::factor()
    {
        double val{unary()};

        if (m_token.type == TokenType::Power)
        {
            scan();
            val = std::pow(val, unary());
        }

        return val;
    }

    // <unary> ::= -<value>
    //      |   <value>
    double Parser::unary()
    {
        int8_t multi{1};

        if (m_token.type == TokenType::Minus)
        {
            scan();
            multi = -1;
        }

        return value() * multi;
    }

    // <value> ::= ( <expr> )
    //      |   <id>
    //      | 	num
    double Parser::value()
    {
        double val{0};

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
            // std::stod might be a problem for precision here.
            val = std::stod(m_token.value);
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
    double Parser::id()
    {
        const Token token{m_token};
        const std::string_view str{token.value};
        scan();

        // Expect a constant
        if (m_token.type != TokenType::LeftParen && token.type == TokenType::Identifier)
        {
            if (str == "pi")
                return 3.14159265;
            else if (str == "e")
                return 2.71828183;
        }

        const std::array<std::pair<std::string_view, std::function<double(double)>>, 9> funcArr{
            std::pair{"log", [](double x) { return std::log(x); }},
            std::pair{"log10", [](double x) { return std::log10(x); }},
            std::pair{"exp", [](double x) { return std::exp(x); }},
            std::pair{"sin", [](double x) { return std::sin(x); }},
            std::pair{"cos", [](double x) { return std::cos(x); }},
            std::pair{"tan", [](double x) { return std::tan(x); }},
            std::pair{"arcsin", [](double x) { return std::asin(x); }},
            std::pair{"arccos", [](double x) { return std::acos(x); }},
            std::pair{"atan", [](double x) { return std::atan(x); }}};

        // Is str a function?
        const auto found = std::find_if(funcArr.cbegin(), funcArr.cend(),
                                  [&](const auto& pair) { return pair.first == str; });
        if (found != funcArr.cend())
        {
            if (m_token.type == TokenType::LeftParen)
            {
                // Should be a valid function here.
                // value() might fail, but that is a valid error.
                return found->second(value());
            }

            // it is a function but missing its starting parentheses.
            error(m_token, "'('");
        }

        // Neither error or function, since '(' is not found we expect a constant.
        error(token, "constant: \"pi\" or \"e\"");
        return 0;
    }

    void Parser::error(const Token& token, const std::string& expected) const
    {
        const std::string unexpected{"token of \"" + std::string{tokenStr(token.type)} + "\""};
        throw ParserError(errorMsg(unexpected, m_scanner.currentLine(), token.location) +
                          ((!expected.empty()) ? "Expected " + expected + "!" : ""));
    }

} // namespace CalcEval