//
//  Scanner.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

#ifndef CALCEVAL_SCANNER_HPP
#define CALCEVAL_SCANNER_HPP

// Local Headers
#include "calceval/Error.hpp"
#include "calceval/Token.hpp"

// C++ Headers
#include <istream>
#include <optional>

namespace CalcEval
{
    /** Scanner class implementation.

        Object to scan an istream for tokens.
        Can throw ScannerError when an error is encountered.
    */
    class Scanner
    {
    public:
        /** Default Scanner constructor is disabled.

            For now, it must be initialized with an istream.
            Might change in the future though.
        */
        Scanner() = delete;

        /** Scanner constructor with iss.

            @param  iss     istringstream to use
            @return         default initialized Scanner
        */
        explicit Scanner(std::istringstream& iss);

        /** Scanner constructor with ifs.

            @param  is      ifstream to use
            @return         default initialized Scanner
        */
        explicit Scanner(std::ifstream& ifs);

        /** Function to scan the istream and return a token.

            If the scanner encounters any value from std::isspace it is
            ignored. Otherwise it returns a token when it finds a valid
            input. If any invalid input is found it will return either
            a token of TokenType::Bad or throw a ScannerError.

            ScannerError can be thrown at any time.

            @return     token
        */
        [[nodiscard]] Token scan();

        /** Retrieve the istream.

            @return     istream
        */
        [[nodiscard]] std::istream& stream() const;

        /** Retrieve the scanned content.

            @return     scanned content
        */
        [[nodiscard]] std::string scanned() const;

        /** Retrieve the location to be scanned.

            Note: This is the location to BE scanned and not the
            last scanned location.

            @return     location to be scanned
        */
        [[nodiscard]] const Location& location() const;

    private:
        /** Function to ignoring the whitespaces in the istream.

            It uses the std::isspace to determine whitespaces.

            @return     true if newline was encountered
        */
        std::optional<TokenType> ignoreWhitespaces();

        /** Function to read identifiers from the istream.

            Identifiers must start with a char from std::isalpha, then
            it can either match a std::isalpha or std::isdigit.

            @return     string with the identifier, empty if some error occurred
        */
        std::string readIdentifier();

        /** Function to read digits from the istream.

            Digits must be a double, it stores them in a std::string
            so precision will be preserved and the Parser or user can
            determine what to do with it.

            @return     string with the identifier, empty if some error occurred
        */
        std::optional<std::string> readDigit();

        /** Function to read a symbol from the istream.

            There are predefined symbols in the definition.
            Symbols must be a char.

            @return     char with associated TokenType, Bad if error
        */
        std::optional<std::pair<char, TokenType>> readSymbol();

        /** Function to throw an error.

            @param  unexpected      what was unexpected
            @param  location        the location it was encountered on
        */
        void error(const std::string& unexpected, const Location& location) const;

    private:
        std::istream& m_stream;
        Location m_cLoc{1, 1}; // Current location
        int m_next{-1};        // Next char in the istream
    };

    /** ScannerError class implementation.

        Object that is thrown when the Scanner encounters an error.
        Use the ScannerError.what() function for details.
    */
    class ScannerError : public Error
    {
    public:
        using Error::Error;
    };

} // namespace CalcEval

#endif // CALCEVAL_SCANNER_HPP
