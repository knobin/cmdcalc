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
#include <utility>

namespace CalcEval
{
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
                if (std::isalpha(m_stream.peek()))
                {
                    throw ScannerError("Unexpected character at {" + std::to_string(m_cLoc.line) +
                                       ", " + std::to_string(m_cLoc.column) + "}");
                }

                return Token(TokenType::Identifier, {m_cLoc.line, col}, identifier);
            }
        }
        else if (std::isdigit(m_next))
        {
            std::size_t col{m_cLoc.column};
            std::string digit{readDigit()};
            if (!digit.empty())
            {
                if (std::isdigit(m_stream.peek()))
                {
                    throw ScannerError("Unexpected digit at {" + std::to_string(m_cLoc.line) +
                                       ", " + std::to_string(m_cLoc.column) + "}");
                }

                return Token(TokenType::Number, {m_cLoc.line, col}, digit);
            }
        }
        else
        {
            // '+', '-', '*', '/', etc
            std::pair<char, TokenType> symbol{readSymbol()};
            if (symbol.second != TokenType::Bad)
            {
                return Token{
                    symbol.second, {m_cLoc.line, m_cLoc.column - 1}, std::string{symbol.first}};
            }
            else
            {
                throw ScannerError("Unexpected symbol '" + std::string{symbol.first} +
                                   "' at line " + std::to_string(m_cLoc.line) + ", column " +
                                   std::to_string(m_cLoc.column - 1));
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

        // First see if we can read a double.
        if (m_stream >> val)
        {
            // To preserve the precision since we store values in a string (to_string has
            // a set precision), we read the value again with seekg and read to store it in
            // string directly.
            std::istream::pos_type bPos{m_stream.tellg()};

            if (bPos == -1)
            {
                m_stream.clear();
                m_stream.seekg(0, std::istream::end);
                bPos = m_stream.tellg();
            }

            if (bPos > aPos)
            {
                std::istream::pos_type length{bPos - aPos};
                std::string str(length, '\0');
                m_stream.seekg(aPos);
                m_stream.read(&str[0], length);
                m_cLoc.column += static_cast<std::size_t>(length);
                return str;
            }
        }

        return "";
    }

    std::pair<char, TokenType> Scanner::readSymbol()
    {
        char symbol{};
        if (m_stream >> symbol)
        {
            ++m_cLoc.column;
            constexpr std::array<std::pair<char, TokenType>, 7> toMatch{
                std::pair{'+', TokenType::Plus},      std::pair{'-', TokenType::Minus},
                std::pair{'*', TokenType::Multiply},  std::pair{'/', TokenType::Divide},
                std::pair{'^', TokenType::Power},     std::pair{'(', TokenType::LeftParen},
                std::pair{')', TokenType::RightParen}};
            auto found = std::find_if(toMatch.cbegin(), toMatch.cend(),
                                      [&](const auto& pair) { return pair.first == symbol; });
            if (found != toMatch.cend())
                return *found;
        }

        return {symbol, TokenType::Bad};
    }

} // namespace CalcEval