//
//  Scanner.cpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

// Local Headers
#include "calceval/Scanner.hpp"

// C++ Headers
#include <array>
#include <iostream>
#include <utility>

namespace CalcEval
{
    Scanner::Scanner(std::istream& is) : m_stream{is}
    {
    }

    Token Scanner::scan()
    {
        if (m_stream.eof())
        {
            return Token{TokenType::EndMark, m_cLoc, ""};
        }

        m_next = m_stream.peek();

        if (ignoreWhitespaces())
        {
            return Token(TokenType::EndOfLine, m_cLoc, "");
        }

        if (std::isalpha(m_next))
        {
            std::size_t col{m_cLoc.column};
            std::string identifier{readIdentifier()};
            if (!identifier.empty())
            {
                return Token(TokenType::Identifier, {m_cLoc.line, col}, identifier);
            }
        }
        else if (std::isdigit(m_next))
        {
            std::size_t col{m_cLoc.column};
            std::string digit{readDigit()};
            if (!digit.empty())
            {
                return Token(TokenType::Number, {m_cLoc.line, col}, digit);
            }
        }
        else
        {
            // '+', '-', '*', '/', etc
            std::pair<char, TokenType> single{readSingle()};
            if (single.second != TokenType::Bad)
            {
                return Token{
                    single.second, {m_cLoc.line, m_cLoc.column - 1}, std::string{single.first}};
            }
        }

        return Token{TokenType::Bad, m_cLoc, ""};
    }

    bool Scanner::ignoreWhitespaces()
    {
        while (std::isspace(m_next))
        {
            ++m_cLoc.column;
            m_stream.ignore(1, m_next);

            if (m_next == '\n')
            {
                m_cLoc.column = 0;
                ++m_cLoc.line;
                m_stream.ignore(1, '\n');
                return true;
            }

            m_next = m_stream.peek();
        }

        return false;
    }

    std::string Scanner::readIdentifier()
    {
        std::string val{};
        do
        {
            char c = ' ';
            if (m_stream >> c)
            {
                val += c;
                ++m_cLoc.column;
            }
        } while (std::isalpha(m_stream.peek()));

        return val;
    }

    std::string Scanner::readDigit()
    {
        double val{0};
        std::istream::pos_type aPos{m_stream.tellg()};
        if (m_stream >> val)
        {
            std::istream::pos_type bPos{m_stream.tellg()};
            if (bPos != -1 && bPos > aPos)
                m_cLoc.column += static_cast<std::size_t>(bPos - aPos);

            return std::to_string(val);
        }

        return "";
    }

    std::pair<char, TokenType> Scanner::readSingle()
    {
        char single{};
        if (m_stream >> single)
        {
            ++m_cLoc.column;
            constexpr std::array<std::pair<char, TokenType>, 7> toMatch{
                std::pair{'+', TokenType::Plus},      std::pair{'-', TokenType::Minus},
                std::pair{'*', TokenType::Multiply},  std::pair{'/', TokenType::Divide},
                std::pair{'^', TokenType::Power},     std::pair{'(', TokenType::LeftParen},
                std::pair{')', TokenType::RightParen}};
            auto found = std::find_if(toMatch.cbegin(), toMatch.cend(),
                                      [&](const auto& pair) { return pair.first == single; });
            if (found != toMatch.cend())
                return *found;
        }

        return {'\0', TokenType::Bad};
    }

} // namespace CalcEval