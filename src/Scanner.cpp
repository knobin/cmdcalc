//
//  Scanner.cpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

// Local Headers
#include "calceval/Scanner.hpp"

// C++ Headers
#include <algorithm>
#include <array>
#include <utility>

namespace CalcEval
{
    Scanner::Scanner(std::istringstream& iss) : m_stream{reinterpret_cast<std::istream&>(iss)}
    {
    }

    Scanner::Scanner(std::ifstream& ifs) : m_stream{reinterpret_cast<std::istream&>(ifs)}
    {
    }

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
            const Location::value_type col{m_cLoc.column};
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
            const Location::value_type col{m_cLoc.column};
            if (auto digit = readDigit())
            {
                if (std::isdigit(m_stream.peek()))
                {
                    error("digit", m_cLoc);
                }

                return {TokenType::Number, {m_cLoc.line, col}, *digit};
            }
        }
        else if (auto symbol = readSymbol())
        {
            if (symbol->second != TokenType::Bad)
            {
                return Token{
                    symbol->second, {m_cLoc.line, m_cLoc.column - 1}, std::string{symbol->first}};
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
                m_cLoc.column = 1;
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

    std::optional<std::string> Scanner::readDigit()
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
                m_cLoc.column += static_cast<Location::value_type>(length);
                return str;
            }
        }

        return std::nullopt;
    }

    std::optional<std::pair<char, TokenType>> Scanner::readSymbol()
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
            const auto search = std::find_if(toMatch.cbegin(), toMatch.cend(),
                                            [&](const auto& pair) { return pair.first == symbol; });
            if (search != toMatch.cend())
                return *search;

            return std::pair{symbol, TokenType::Bad};
        }

        return std::nullopt;
    }

    std::istream& Scanner::stream() const
    {
        return m_stream;
    }

    static std::istream::pos_type getStreamPos(std::istream& is)
    {
        std::istream::pos_type pos{is.tellg()};

        // Assuming if pos is -1 that the stream is passed the end.
        if (pos == -1)
        {
            is.clear();
            is.seekg(0, std::istream::end);
            pos = is.tellg();
        }

        return pos;
    }

    std::string Scanner::scanned() const
    {
        const std::istream::pos_type currentPos{getStreamPos(m_stream)};
        std::string line(static_cast<std::size_t>(currentPos), '\0');

        m_stream.clear();
        m_stream.seekg(std::ios::beg);
        m_stream.read(&line[0], currentPos);

        m_stream.clear();
        m_stream.seekg(currentPos); // return to old pos

        return line;
    }

    const Location& Scanner::location() const
    {
        return m_cLoc;
    }

    void Scanner::error(const std::string& unexpected, const Location& location) const
    {
        std::string content{scanned()};
        std::size_t start{0};
        const std::size_t findLast{content.find_last_of('\n')};
        if (findLast != std::string::npos)
            start = findLast + 1;

        const std::string line{content.substr(start)};
        throw ScannerError(line, location, unexpected);
    }

} // namespace CalcEval
