//-------------------------------------------------------------------------------//
//                         QUIMERA ENGINE : LICENSE                              //
//-------------------------------------------------------------------------------//
// This file is part of Quimera Engine.                                          //
// Quimera Engine is free software: you can redistribute it and/or modify        //
// it under the terms of the Lesser GNU General Public License as published by   //
// the Free Software Foundation, either version 3 of the License, or             //
// (at your option) any later version.                                           //
//                                                                               //
// Quimera Engine is distributed in the hope that it will be useful,             //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// Lesser GNU General Public License for more details.                           //
//                                                                               //
// You should have received a copy of the Lesser GNU General Public License      //
// along with Quimera Engine. If not, see <http://www.gnu.org/licenses/>.        //
//                                                                               //
// This license doesn't force you to put any kind of banner or logo telling      //
// that you are using Quimera Engine in your project but we would appreciate     //
// if you do so or, at least, if you let us know about that.                     //
//                                                                               //
// Enjoy!                                                                        //
//                                                                               //
// Kinesis Team                                                                  //
//-------------------------------------------------------------------------------//

#ifndef __UNICODECOLLATIONTESTDATA_H__
#define __UNICODECOLLATIONTESTDATA_H__

#include "DataTypesDefinitions.h"

#include "QStringUnicode.h"
#include "QCharUnicode.h"
#include <fstream>
#include <math.h>

using Kinesis::QuimeraEngine::Common::DataTypes::string_q;
using Kinesis::QuimeraEngine::Common::DataTypes::codepoint_q;
using Kinesis::QuimeraEngine::Common::DataTypes::u32_q;
using Kinesis::QuimeraEngine::Common::DataTypes::QStringUnicode;
using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;
using Kinesis::QuimeraEngine::Common::DataTypes::EQTextEncoding;

namespace Kinesis
{
namespace QuimeraEngine
{
namespace Test
{
namespace Collation
{

/// <summary>
/// A Unicode UTF32 character sequence.
/// </summary>
struct QCharacterSequenceTest
{
    /// <summary>
    /// The number of codepoints in the sequence. The case with more code unit has 6 of them.
    /// </summary>
    static const unsigned int MAXIMUM_NUMBER_OF_CODEPOINTS = 6;

    /// <summary>
    /// A Unicode UTF32 character sequence.
    /// </summary>
    u32_q CodePoints[MAXIMUM_NUMBER_OF_CODEPOINTS];

    /// <summary>
    /// The length of the sequence.
    /// </summary>
    u32_q ArraySize;
};

/// <summary>
/// A row in the test file, containing 1 column.
/// </summary>
struct QCollationTest
{
    /// <summary>
    /// A row in the test file, containing 1 column.
    /// </summary>
    QCharacterSequenceTest CharacterSequences[1];
};

/// <summary>
/// Parser that converts the test file provided in the Unicode Standard annexes http://www.unicode.org/Public/UCA/6.3.0/CollationTest.zip.
/// Every character sequence appears in one line, and is greater than the previous sequence.
/// The collation stregth must be IDENTICAL, so this cannot be used to test our classes.
/// </summary>
class QCollationTestFileParser
{
protected:

    /// <summary>
    /// The maximum length of a code unit in all the cases.
    /// </summary>
    static const unsigned int CODEPOINT_LENGTH = 6;

public:
    
    /// <summary>
    /// Converts the text of the test file into one array that contains all the test cases.
    /// </summary>
    /// <param name="strFilePath">[IN] The path of the file.</param>
    void ParseFile(const QStringUnicode &strFilePath)
    {
        unsigned uLength = 0;
        std::fstream fileStream;
        fileStream.open(strFilePath.ToBytes(EQTextEncoding::E_ISO88591, uLength));

        QE_ASSERT(fileStream.is_open(), "The Collation test file was not found");

        if(fileStream.is_open())
        {
            fileStream.seekg(0, fileStream.end);
            unsigned int uLength = scast_q(fileStream.tellg(), unsigned int);
            fileStream.seekg(0, fileStream.beg);

            char* pBuffer = new char[uLength];
            fileStream.read(pBuffer, uLength);
            fileStream.close();

            QStringUnicode strFileContent(pBuffer, uLength, EQTextEncoding::E_UTF8);

            this->ParseString(strFileContent);

            delete[] pBuffer;
        }
    }

    /// <summary>
    /// Gets the tests cases.
    /// </summary>
    const std::vector<QCollationTest>& GetTestCases() const
    {
        return m_arTestCases;
    }

protected:

    /// <summary>
    /// Converts the text into several arrays, one per section, that contain all the test cases.
    /// </summary>
    /// <param name="strInput">[IN] A string containing test case data.</param>
    void ParseString(const QStringUnicode &strInput)
    {
        static const QCharUnicode COMMENT_CHAR('#');
        static const QCharUnicode NEWLINE_CHAR('\n');

        const unsigned int INPUT_LENGTH = strInput.GetLength();

        QCharUnicode firstCharInLine(0);
        QCharUnicode currentChar(0);

        QStringUnicode::QConstCharIterator it = strInput.GetConstCharIterator();
        QStringUnicode::QConstCharIterator itFirstInLine = it;
        QCollationTest collationTest;
        bool bWasFilled = false;

        while(!it.IsEnd())
        {
            currentChar = it.GetChar();

            if(currentChar == NEWLINE_CHAR) // End of the current line
            {
                firstCharInLine = itFirstInLine.GetChar();

                if(firstCharInLine != COMMENT_CHAR) // Skips comments
                {
                    bWasFilled = this->ParseLine(strInput, collationTest, itFirstInLine);

                    if(bWasFilled)
                    {
                        // Adds the test
                        m_arTestCases.push_back(collationTest);
                    }
                }

                itFirstInLine = it;
                ++itFirstInLine;
            }

            ++it;
        }
    }

    /// <summary>
    /// Parses the current line, starting from the iterator and filling the collation test structure.
    /// </summary>
    /// <param name="strLine">[IN] The text line.</param>
    /// <param name="collationTest">[OUT] The collation test case struncture to fill.</param>
    /// <param name="it">[OUT] The iterator that points to the current position in the text. It will be modified, pointing to the end of the line at the end of the process.</param>
    /// <returns>
    /// True if the collation test case structure was filled; False if nothing was done.
    /// </returns>
    bool ParseLine(const QStringUnicode &strLine, QCollationTest &collationTest, QStringUnicode::QConstCharIterator &it)
    {
        static const QCharUnicode COMMENT_CHAR('#');
        static const QCharUnicode WHITESPACE_CHAR(' ');
        static const QCharUnicode NEWLINE_CHAR('\n');
        static const QCharUnicode SEMICOLON_CHAR(';');
        static const QCharUnicode BOMLE_CHAR(0xFEFF);
        static const QCharUnicode BOMBE_CHAR(0xFFFE);

        if(strLine.IsEmpty() || it.GetChar() == COMMENT_CHAR) // Skips comment lines
            return false;

        QCharacterSequenceTest sequence;
        unsigned int uCurrentColumn = 0;
        unsigned int uCurrentCodePoint = 0;
        bool bFilled = false;

        QCharUnicode currentChar(0);
        QStringUnicode strCodePoint;

        while(!it.IsEnd())
        {
            currentChar = it.GetChar();

            if(currentChar == WHITESPACE_CHAR) // Next code point
            {
                ++uCurrentCodePoint;
                ++it;
            }
            else if(currentChar == SEMICOLON_CHAR) // Next column
            {
                // Adds the sequence
                collationTest.CharacterSequences[uCurrentColumn] = sequence;
                bFilled = true;
                ++uCurrentColumn;
                ++it;
                uCurrentCodePoint = 0;
            }
            else if(currentChar == COMMENT_CHAR || currentChar == NEWLINE_CHAR) // Skips comments at the end of the line
            {
                break;
            }
            else if(currentChar != BOMBE_CHAR && currentChar != BOMLE_CHAR)
            {
                // Adds the code point
                sequence.CodePoints[uCurrentCodePoint] = this->ConvertStringToCodePoint(strLine, it);;
                sequence.ArraySize = uCurrentCodePoint + 1;
            }
            else
            {
                ++it;
            }
        }

        return bFilled;
    }

    /// <summary>
    /// Parses the current code point, starting from the iterator and returning the corresponding number.
    /// </summary>
    /// <param name="strCodePoint">[IN] The text that contains the code point.</param>
    /// <param name="it">[OUT] The iterator that points to the current position in the text. It will be modified, pointing to the end of the code point at the end of the process.</param>
    /// <returns>
    /// The number read from the text.
    /// </returns>
    u32_q ConvertStringToCodePoint(const QStringUnicode &strCodePoint, QStringUnicode::QConstCharIterator &it) const
    {
        static const QCharUnicode HEX_1_CHAR = '1';
        static const QCharUnicode HEX_2_CHAR = '2';
        static const QCharUnicode HEX_3_CHAR = '3';
        static const QCharUnicode HEX_4_CHAR = '4';
        static const QCharUnicode HEX_5_CHAR = '5';
        static const QCharUnicode HEX_6_CHAR = '6';
        static const QCharUnicode HEX_7_CHAR = '7';
        static const QCharUnicode HEX_8_CHAR = '8';
        static const QCharUnicode HEX_9_CHAR = '9';
        static const QCharUnicode HEX_A_CHAR = 'A';
        static const QCharUnicode HEX_B_CHAR = 'B';
        static const QCharUnicode HEX_C_CHAR = 'C';
        static const QCharUnicode HEX_D_CHAR = 'D';
        static const QCharUnicode HEX_E_CHAR = 'E';
        static const QCharUnicode HEX_F_CHAR = 'F';
        static const QCharUnicode SEPARATOR1_CHAR = ';';
        static const QCharUnicode SEPARATOR2_CHAR = ' ';

        u32_q result = 0;
        QCharUnicode currentChar(0);

        // First, gets the length of the code unit, with a minimum of 4 bytes
        unsigned int uLength = 0;
        QStringUnicode::QConstCharIterator iCodeUnit = it;

        while( uLength < QCollationTestFileParser::CODEPOINT_LENGTH && (uLength < 3 || (it.GetChar() != SEPARATOR1_CHAR && it.GetChar() != SEPARATOR2_CHAR)) )
        {
            ++uLength;
            ++it;
        }

        --uLength; // zero-based

        // Traverses every character of the code unit until its length
        for(unsigned int i = 0; i < QCollationTestFileParser::CODEPOINT_LENGTH; ++i)
        {
            currentChar = iCodeUnit.GetChar();

            if(     currentChar == HEX_1_CHAR)
                result += 0x1U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_2_CHAR)
                result += 0x2U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_3_CHAR)
                result += 0x3U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_4_CHAR)
                result += 0x4U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_5_CHAR)
                result += 0x5U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_6_CHAR)
                result += 0x6U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_7_CHAR)
                result += 0x7U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_8_CHAR)
                result += 0x8U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_9_CHAR)
                result += 0x9U * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_A_CHAR)
                result += 0xAU * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_B_CHAR)
                result += 0xBU * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_C_CHAR)
                result += 0xCU * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_D_CHAR)
                result += 0xDU * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_E_CHAR)
                result += 0xEU * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );
            else if(currentChar == HEX_F_CHAR)
                result += 0xFU * scast_q( pow(16.0f, float(uLength - i)) , unsigned int );

            ++iCodeUnit;
        }

        return result;
    }

    /// <summary>
    /// The tests cases.
    /// </summary>
    std::vector<QCollationTest> m_arTestCases;

};

} //namespace Collation
} //namespace Test
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __UNICODECOLLATIONTESTDATA_H__
