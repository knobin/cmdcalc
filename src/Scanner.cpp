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
#include <algorithm>
#include <utility>

namespace CalcEval
{
    Token Scanner::scan()
    {
        m_next = m_stream.peek();

        if (auto type = ignoreWhitespaces())
        {
            // EndMark or EndOfLine returned
            return Token{*type, m_cLoc, ""};
        }

        if (std::isalpha(m_next))
        {
            const std::size_t col{m_cLoc.column};
            const std::string identifier{readIdentifier()};
            if (!identifier.empty())
            {
                if (std::isalpha(m_stream.peek()))
                {
                    error("character", m_cLoc);
                }

                return Token(TokenType::Identifier, {m_cLoc.line, col}, identifier);
            }
        }
        else if (std::isdigit(m_next))
        {
            const std::size_t col{m_cLoc.column};
            const std::string digit{readDigit()};
            if (!digit.empty())
            {
                if (std::isdigit(m_stream.peek()))
                {
                    error("digit", m_cLoc);
                }

                return Token(TokenType::Number, {m_cLoc.line, col}, digit);
            }
        }
        else
        {
            // '+', '-', '*', '/', etc
            const std::pair<char, TokenType> symbol{readSymbol()};
            if (symbol.second != TokenType::Bad)
            {
                return Token{
                    symbol.second, {m_cLoc.line, m_cLoc.column - 1}, std::string{symbol.first}};
            }
            else
            {
                error("symbol", {m_cLoc.line, m_cLoc.column - 1});
            }
        }

        return Token{TokenType::Bad, m_cLoc, ""};
    }

    std::optional<TokenType> Scanner::ignoreWhitespaces()
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
                return TokenType::EndOfLine;
            }

            m_next = m_stream.peek();
        }

        if (m_stream.eof() || m_next < 0)
        {
            return TokenType::EndMark;
        }

        return std::nullopt;
    }

    std::string Scanner::readIdentifier()
    {
        // We have at least one std::isalpha here.
        std::string val{};
        int next;
        do
        {
            char c = ' ';
            if (m_stream >> c)
            {
                val += c;
                ++m_cLoc.column;
            }
            // Maybe check for error here?
            next = m_stream.peek();
        } while (std::isalpha(next) || std::isdigit(next));

        return val;
    }

    std::string Scanner::readDigit()
    {
        double val{0};
        const std::istream::pos_type aPos{m_stream.tellg()};

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
                const std::istream::pos_type length{bPos - aPos};
                std::string str(static_cast<std::size_t>(length), '\0');
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
            const auto found = std::find_if(toMatch.cbegin(), toMatch.cend(),
                                      [&](const auto& pair) { return pair.first == symbol; });
            if (found != toMatch.cend())
                return *found;
        }

        return {symbol, TokenType::Bad};
    }

    void Scanner::error(const std::string& unexpected, const Location& location) const
    {
        throw ScannerError(errorMsg(unexpected, currentLine(), location));
    }

    std::string Scanner::currentLine() const
    {
        // Save current position
        std::istream::pos_type pos{m_stream.tellg()};
        if (pos == -1)
        {
            m_stream.clear();
            m_stream.seekg(0, std::istream::end);
            pos = m_stream.tellg();
        }

        // Find '\n' or beginning
        int prevChar;
        std::istream::pos_type currPos{pos};
        do
        {
            m_stream.seekg(-1, std::istream::cur);
            currPos = m_stream.tellg();
            prevChar = m_stream.get();
            m_stream.seekg(-1, std::istream::cur);
        } while (prevChar != '\n' && currPos > 0);

        // Could find "end" here as well to get the whole line instead of just the beginning.

        // Retrieve line
        const std::istream::pos_type length{pos - currPos};
        std::string line(static_cast<std::size_t>(length), '\0');
        m_stream.seekg(currPos);
        m_stream.read(&line[0], length);

        m_stream.seekg(pos); // return to old pos

        return line;
    }

    std::string errorMsg(const std::string& unexpected, const std::string& line,
                         const Location& location)
    {
        return {"Unexpected " + unexpected + " at line " + std::to_string(location.line) + " : " +
                std::to_string(location.column) + "\n" + errorFromLine(line, location.column)};
    }

    std::string errorFromLine(const std::string& line, std::size_t at)
    {
        return line + '\n' + ((at > 1) ? std::string(at - 1, '-') : "") + "^\n";
    }

} // namespace CalcEval