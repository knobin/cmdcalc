//
//  Parser.cpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

// Local Headers
#include "calceval/Parser.hpp"

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
            val = expr();

        scan();

        if (m_token.type != TokenType::EndMark)
            throw ParserError("There are symbols left to parse!");

        return val;
    }

    double Parser::expr()
    {
        double lhs{term()};
        return exprTail(lhs);
    }

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

    double Parser::term()
    {
        double lhs{factor()};
        return termTail(lhs);
    }

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

    double Parser::value()
    {
        double val{0};

        if (m_token.type == TokenType::LeftParen)
        {
            scan();
            val = expr();
            if (m_token.type != TokenType::RightParen)
            {
                throw ParserError("Expected ')'!");
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
            throw ParserError("Unexpected token!");
        }

        scan();
        return val;
    }

    double Parser::id()
    {
        std::string str{m_token.value};
        scan();

        if (m_token.type != TokenType::LeftParen)
        {
            if (str == "pi")
                return 3.14159265;
            else if (str == "e")
                return 2.71828183;

            throw ParserError("No constant named \"" + str + "\" found!");
        }

        if (str == "log")
            return std::log(value());
        if (str == "log10")
            return std::log10(value());
        if (str == "exp")
            return std::exp(value());
        if (str == "sin")
            return std::sin(value());
        if (str == "cos")
            return std::cos(value());
        if (str == "tan")
            return std::tan(value());
        if (str == "arcsin")
            return std::asin(value());
        if (str == "arccos")
            return std::acos(value());
        if (str == "arctan")
            return std::atan(value());

        throw ParserError("No function named \"" + str + "\" found!");

        return 0;
    }

} // namespace CalcEval