﻿//-------------------------------------------------------------------------------//
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

#include "QStringUnicode.h"

#include <cstring> // Needed for strlen function
#include "Assertions.h"
#include "EQComparisonType.h"
#include "EQNormalizationForm.h"
#include "EQTextEncoding.h"

#include <unicode/ucnv.h>
#include <unicode/fmtable.h>
#include <unicode/decimfmt.h>
#include "unicode/locid.h"
#include "unicode/uchar.h"


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Common
{
namespace DataTypes
{

//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |  ATTRIBUTES INITIALIZATION |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

const int QStringUnicode::LENGTH_NULL_TERMINATED = -1;
const QCharUnicode QStringUnicode::CHAR_BOM_LE(0xFFFE);
const QCharUnicode QStringUnicode::CHAR_BOM_BE(0xFEFF);
const int QStringUnicode::PATTERN_NOT_FOUND = -1;


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |       CONSTRUCTORS		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

QStringUnicode::QStringUnicode() : m_strString()
{
}

QStringUnicode::QStringUnicode(const QStringUnicode &strString)
{
    m_strString = strString.m_strString;
}

QStringUnicode::QStringUnicode(const i8_q* arBytes)
{
    // CAUTION: This is just a workaround to avoid performance loss due to unncecessry class instancing
    //          If this class inherited from another, a different solution should be applied (calling a common function)
    //          It is not possible to call other constructors until C++11
    new (this) QStringUnicode(arBytes, QStringUnicode::LENGTH_NULL_TERMINATED, EQTextEncoding::E_ISO88591);
}

QStringUnicode::QStringUnicode(const i8_q* arBytes, const int nLength)
{
    // CAUTION: This is just a workaround to avoid performance loss due to unncecessry class instancing
    //          If this class inherited from another, a different solution should be applied (calling a common function)
    //          It is not possible to call other constructors until C++11
    new (this) QStringUnicode(arBytes, nLength, EQTextEncoding::E_ISO88591);
}

QStringUnicode::QStringUnicode(const i8_q* arBytes,
                               const int nLength,
                               const EQTextEncoding &eEncoding)
{
    // Only ASCII and ISO 8859-1 encodings can be used along with null-terminated strings' length calculation
    QE_ASSERT(((eEncoding == EQTextEncoding::E_ASCII || eEncoding == EQTextEncoding::E_ISO88591) &&
              nLength == QStringUnicode::LENGTH_NULL_TERMINATED) ||
              nLength != QStringUnicode::LENGTH_NULL_TERMINATED, "Only ASCII and ISO 8859-1 encodings can be used along with null-terminated strings' length calculation");

    UErrorCode errorCode = U_ZERO_ERROR;
    UConverter* pConverter = QStringUnicode::GetConverter(eEncoding);

    int nActualLength = nLength;

    if(nLength == QStringUnicode::LENGTH_NULL_TERMINATED &&
       (eEncoding == EQTextEncoding::E_ASCII || eEncoding == EQTextEncoding::E_ISO88591))
        nActualLength = strlen(arBytes);

    m_strString = icu::UnicodeString(arBytes, nActualLength, pConverter, errorCode);
}

QStringUnicode::QStringUnicode(const QCharUnicode &character) : m_strString(UChar(character.GetCodePoint()))
{
}


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |		    METHODS			 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

QStringUnicode& QStringUnicode::operator=(const QStringUnicode &strString)
{
    // [TODO] Thund: Currently, this operator will invalidate any interator that may be pointing to the string.
    //               In the future we should look for a solution, if there is any.
    m_strString = strString.m_strString;
    return *this;
}

bool QStringUnicode::operator==(const QStringUnicode &strString) const
{
    return (m_strString == strString.m_strString) != FALSE;
}

bool QStringUnicode::operator!=(const QStringUnicode &strString) const
{
    return (m_strString != strString.m_strString) != FALSE;
}

bool QStringUnicode::operator<(const QStringUnicode &strString) const
{
    return (m_strString < strString.m_strString) != FALSE;
}

bool QStringUnicode::operator<=(const QStringUnicode &strString) const
{
    return (m_strString <= strString.m_strString) != FALSE;
}

bool QStringUnicode::operator>(const QStringUnicode &strString) const
{
    return (m_strString > strString.m_strString) != FALSE;
}

bool QStringUnicode::operator>=(const QStringUnicode &strString) const
{
    return (m_strString >= strString.m_strString) != FALSE;
}

QStringUnicode QStringUnicode::operator+(const QStringUnicode &strString) const
{
    QStringUnicode strNewString;
    strNewString.m_strString =  m_strString;
    strNewString.m_strString += strString.m_strString;
    return strNewString;
}

QCharUnicode QStringUnicode::operator[](const unsigned int uIndex) const
{
    // It is not possible to retrieve any character if the string is empty
    QE_ASSERT(!this->IsEmpty(), "It is not possible to retrieve any character if the string is empty");

    // Index out of bounds: The index must be lower than the length of the string
    QE_ASSERT(!this->IsEmpty() && uIndex < this->GetLength(), "Index out of bounds: The index must be lower than the length of the string");

    QCharUnicode charUnicode = m_strString.char32At(uIndex);
    return charUnicode;
}

QStringUnicode QStringUnicode::Substring(const unsigned int uStartPosition, const unsigned int uLastPosition) const
{
    // The start position index must be lower or equal than the last position index
    QE_ASSERT(uStartPosition <= uLastPosition, "The start position index must be lower or equal than the last position index");

    QStringUnicode strResult;
    m_strString.extract(uStartPosition, uLastPosition - uStartPosition, strResult.m_strString);

    return strResult;
}

QStringUnicode::QConstCharIterator QStringUnicode::GetConstCharIterator() const
{
    return QStringUnicode::QConstCharIterator(*this);
}

i8_q* QStringUnicode::ToBytes(const EQTextEncoding &eEncoding, unsigned int &uOutputLength) const
{
    i8_q* pOutputBytes = null_q;
    uOutputLength = 0;

    const unsigned int CHARACTERS_COUNT = m_strString.countChar32(); // It does not include the final null character

    if(CHARACTERS_COUNT > 0)
    {
        UErrorCode errorCode = U_ZERO_ERROR;
        UConverter* pConverter = QStringUnicode::GetConverter(eEncoding);
        const unsigned int CODE_UNITS_COUNT = m_strString.length(); // It does not include the final null character

        // Depending on whether the string is already null-terminated or not, a null terminator will be added at the end
        // of the resultant array of bytes
        const unsigned int ADD_NULL_TERMINATION = m_strString.char32At(CHARACTERS_COUNT - 1) == 0 ? 0 : 1;

        // By default, it is assigned as if it was to be encoded in ASCII or ISO 8859-1 (8-bits per character)
        int32_t nRequiredLengthBytes = CHARACTERS_COUNT + ADD_NULL_TERMINATION;

        // Output size calculation for Unicode encoding forms
        switch(eEncoding)
        {
        case EQTextEncoding::E_UTF8:
            // It is not possible to know in advance how much memory the UTF-8 will require
            // (each character could be represented by 1, 2, 3 or 4 8-bits code units) so we reserve the maximum it would need
            nRequiredLengthBytes = sizeof(i32_q) * (CHARACTERS_COUNT + ADD_NULL_TERMINATION);
            break;
        case EQTextEncoding::E_UTF16:
            // We already know the number of 16 bits code units. A BOM character is added at the beginning
            nRequiredLengthBytes = sizeof(i16_q) * (CODE_UNITS_COUNT + 1 + ADD_NULL_TERMINATION);
            break;
        case EQTextEncoding::E_UTF16BE:
        case EQTextEncoding::E_UTF16LE:
            // We already know the number of 16 bits code units
            nRequiredLengthBytes = sizeof(i16_q) * (CODE_UNITS_COUNT + ADD_NULL_TERMINATION);
            break;
        case EQTextEncoding::E_UTF32:
            // The width of UTF32 characters is always 32 bits. A BOM character is added at the beginning
            nRequiredLengthBytes = sizeof(i32_q) * (CHARACTERS_COUNT + 1 + ADD_NULL_TERMINATION);
            break;
        case EQTextEncoding::E_UTF32BE:
        case EQTextEncoding::E_UTF32LE:
            // The width of UTF32 characters is always 32 bits
            nRequiredLengthBytes = sizeof(i32_q) * (CHARACTERS_COUNT + ADD_NULL_TERMINATION);
            break;
        }

        // Conversion from native encoding (UTF16) to input encoding
        const UChar* pBuffer = m_strString.getBuffer();
        pOutputBytes = new char[nRequiredLengthBytes];
        ucnv_reset(pConverter);
        uOutputLength = ucnv_fromUChars(pConverter, pOutputBytes, nRequiredLengthBytes, pBuffer, CODE_UNITS_COUNT, &errorCode);

        // If it was necessary to add a null terminator...
        if(ADD_NULL_TERMINATION == 1)
        {
            // The last character has to be set to zero (ICU adds only 1 byte at the end as the null terminator)
            // The last character has to be added to the output length
            switch(eEncoding)
            {
            case EQTextEncoding::E_ASCII:
            case EQTextEncoding::E_ISO88591:
            case EQTextEncoding::E_UTF8:
                // 8 bits character
                uOutputLength += sizeof(i8_q);
                memset(&pOutputBytes[uOutputLength - sizeof(i8_q)], 0, sizeof(i8_q));
                break;
            case EQTextEncoding::E_UTF16:
            case EQTextEncoding::E_UTF16BE:
            case EQTextEncoding::E_UTF16LE:
                // 16 bits character
                uOutputLength += sizeof(i16_q);
                memset(&pOutputBytes[uOutputLength - sizeof(i16_q)], 0, sizeof(i16_q));
                break;
            case EQTextEncoding::E_UTF32:
            case EQTextEncoding::E_UTF32BE:
            case EQTextEncoding::E_UTF32LE:
                // 32 bits character
                uOutputLength += sizeof(i32_q);
                memset(&pOutputBytes[uOutputLength - sizeof(i32_q)], 0, sizeof(i32_q));
                break;
            }
        }
    }

    return pOutputBytes;
}

UConverter* QStringUnicode::GetConverter(const EQTextEncoding &eEncoding)
{
    // About ICU converters: http://userguide.icu-project.org/conversion/converters
    static UErrorCode errorCode = U_ZERO_ERROR;
    static UConverter* const ASCII_CONVERTER    = ucnv_open("US-ASCII",    &errorCode);
    static UConverter* const ISO88591_CONVERTER = ucnv_open("ISO-8859-1",  &errorCode);
    static UConverter* const UTF8_CONVERTER     = ucnv_open("UTF8",        &errorCode);
    static UConverter* const UTF16_CONVERTER    = ucnv_open("UTF16",       &errorCode);
    static UConverter* const UTF32_CONVERTER    = ucnv_open("UTF32",       &errorCode);
    static UConverter* const UTF16BE_CONVERTER  = ucnv_open("UTF16BE",     &errorCode);
    static UConverter* const UTF16LE_CONVERTER  = ucnv_open("UTF16LE",     &errorCode);
    static UConverter* const UTF32BE_CONVERTER  = ucnv_open("UTF32BE",     &errorCode);
    static UConverter* const UTF32LE_CONVERTER  = ucnv_open("UTF32LE",     &errorCode);

    UConverter* pConverter = null_q;

    switch(eEncoding)
    {
    case EQTextEncoding::E_ASCII:
        pConverter = ASCII_CONVERTER;
        break;
    case EQTextEncoding::E_ISO88591:
        pConverter = ISO88591_CONVERTER;
        break;
    case EQTextEncoding::E_UTF8:
        pConverter = UTF8_CONVERTER;
        break;
    case EQTextEncoding::E_UTF16:
        pConverter = UTF16_CONVERTER;
        break;
    case EQTextEncoding::E_UTF32:
        pConverter = UTF32_CONVERTER;
        break;
    case EQTextEncoding::E_UTF16BE:
        pConverter = UTF16BE_CONVERTER;
        break;
    case EQTextEncoding::E_UTF16LE:
        pConverter = UTF16LE_CONVERTER;
        break;
    case EQTextEncoding::E_UTF32BE:
        pConverter = UTF32BE_CONVERTER;
        break;
    case EQTextEncoding::E_UTF32LE:
        pConverter = UTF32LE_CONVERTER;
        break;
    }

    return pConverter;
}

QStringUnicode QStringUnicode::ToLowerCase() const
{
    QStringUnicode strLowerCase(*this);
    // Use English as locale.
    const Locale &en = Locale::getEnglish();
    strLowerCase.m_strString.toLower(en);
    return strLowerCase;
}

QStringUnicode QStringUnicode::ToUpperCase() const
{
    QStringUnicode strUpperCase(*this);
    // Use English as locale.
    const Locale &en = Locale::getEnglish();
    strUpperCase.m_strString.toUpper(en);
    return strUpperCase;
}

QStringUnicode QStringUnicode::ToCaseFolded() const
{
    QStringUnicode strFoldedCase(*this);
    strFoldedCase.m_strString.foldCase(U_FOLD_CASE_DEFAULT);
    return strFoldedCase;
}

void QStringUnicode::Normalize(const EQNormalizationForm &eNormalizationForm)
{
    const icu::Normalizer2* pNormalizer = QStringUnicode::GetNormalilzer(eNormalizationForm);
    UErrorCode eErrorCode = U_ZERO_ERROR;
    
    UBool bIsNormalized = pNormalizer->isNormalized(m_strString, eErrorCode); // [TODO] Thund: Change this by a call to this->IsNormalized when implemented
    QE_ASSERT(U_SUCCESS(eErrorCode), "An error occurred when checking if the string was normalized"); // And this also.

    if(!bIsNormalized && U_SUCCESS(eErrorCode) != FALSE)
    {
        icu::UnicodeString strNormalized;
        pNormalizer->normalize(m_strString, strNormalized, eErrorCode);

        QE_ASSERT(U_SUCCESS(eErrorCode), "An error occurred when normalizing the string");

        if(U_SUCCESS(eErrorCode) != FALSE)
            m_strString = strNormalized;
    }
}

const icu::Normalizer2* QStringUnicode::GetNormalilzer(const EQNormalizationForm &eNormalizationForm)
{
    const icu::Normalizer2* pNormalizer = null_q;
    UErrorCode eErrorCode = U_ZERO_ERROR;

    switch(eNormalizationForm)
    {
    case EQNormalizationForm::E_C:
        pNormalizer = icu::Normalizer2::getNFCInstance(eErrorCode);
        break;
    case EQNormalizationForm::E_D:
        pNormalizer = icu::Normalizer2::getNFDInstance(eErrorCode);
        break;
    case EQNormalizationForm::E_KC:
    case EQNormalizationForm::E_KD:
        QE_ASSERT(false, "Normalization forms KC and KD are not supported currently");
        break;
    }

    QE_ASSERT(!U_FAILURE(eErrorCode), "An error ocurred when attempting to get a normalizer");

    return pNormalizer;
}

int QStringUnicode::CompareTo(const QStringUnicode &strInputString, const EQComparisonType &eComparisonType) const
{
    int nResult = 0;

    switch(eComparisonType)
    {
    case EQComparisonType::E_BinaryCaseSensitive:
        nResult = m_strString.compare(strInputString.m_strString);
        break;
    case EQComparisonType::E_BinaryCaseInsensitive:
        nResult = m_strString.caseCompare(strInputString.m_strString, U_FOLD_CASE_DEFAULT);
        break;
    case EQComparisonType::E_CanonicalCaseInsensitive:
        {
            const icu::Collator* pCollator = QStringUnicode::GetCollator(eComparisonType);
            nResult = pCollator->compare(m_strString, strInputString.m_strString);
        }
        break;
    case EQComparisonType::E_CanonicalCaseSensitive:
        {
            const icu::Collator* pCollator = QStringUnicode::GetCollator(eComparisonType);
            nResult = pCollator->compare(m_strString, strInputString.m_strString);
        }
        break;
    case EQComparisonType::E_CompatibilityCaseInsensitive:
    case EQComparisonType::E_CompatibilityCaseSensitive:
        QE_ASSERT(false, "Compatibility comparisons are not supported yet");
        break;
    }

    return nResult;
}

const icu::Collator* QStringUnicode::GetCollator(const EQComparisonType &eComparisonType)
{
    // Information of interest: http://userguide.icu-project.org/collation/architecture
    static bool bInitialized = false;
    static UErrorCode errorCode = U_ZERO_ERROR;
    // By default, the collation is English
    static icu::Collator* CANONICAL_CASESENSITIVE_COLLATOR = icu::Collator::createInstance(icu::Locale::getEnglish(), errorCode);
    static icu::Collator* CANONICAL_CASEINSENSITIVE_COLLATOR = icu::Collator::createInstance(icu::Locale::getEnglish(), errorCode);

    QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when getting the collator");

    if(!bInitialized)
    {
        CANONICAL_CASESENSITIVE_COLLATOR->setStrength(icu::Collator::TERTIARY);
        CANONICAL_CASESENSITIVE_COLLATOR->setAttribute(UCOL_NORMALIZATION_MODE, UCOL_ON, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute");
        CANONICAL_CASESENSITIVE_COLLATOR->setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_NON_IGNORABLE, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute");

        CANONICAL_CASEINSENSITIVE_COLLATOR->setStrength(icu::Collator::SECONDARY); // Secondary: No tertiary checking, no case comparison
        CANONICAL_CASEINSENSITIVE_COLLATOR->setAttribute(UCOL_NORMALIZATION_MODE, UCOL_ON, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute");
        CANONICAL_CASEINSENSITIVE_COLLATOR->setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_NON_IGNORABLE, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute");

        bInitialized = true;
    }

    icu::Collator* pCollator = null_q;

    switch(eComparisonType)
    {
    case EQComparisonType::E_CanonicalCaseInsensitive:
        pCollator = CANONICAL_CASEINSENSITIVE_COLLATOR;
        break;
    case EQComparisonType::E_CanonicalCaseSensitive:
        pCollator = CANONICAL_CASESENSITIVE_COLLATOR;
        break;
    case EQComparisonType::E_CompatibilityCaseInsensitive:
    case EQComparisonType::E_CompatibilityCaseSensitive:
        QE_ASSERT(false, "Compatibility comparisons are not supported yet");
        break;
    default:
        QE_ASSERT(false, "Invalid comparison type");
    }

    return pCollator;
}

int QStringUnicode::IndexOf(const QStringUnicode &strPattern, const EQComparisonType &eComparisonType) const
{
    int32_t nPosition = QStringUnicode::PATTERN_NOT_FOUND;

    if(!strPattern.IsEmpty() && !this->IsEmpty())
    {
        if(eComparisonType == EQComparisonType::E_BinaryCaseSensitive)
        {
            nPosition = m_strString.indexOf(strPattern.m_strString);
        }
        else if(eComparisonType == EQComparisonType::E_BinaryCaseInsensitive)
        {
            // There is no case insensitive indexOf in ICU so both strings are case-folded before searching
            icu::UnicodeString strResidentCopy = m_strString;
            strResidentCopy.foldCase(U_FOLD_CASE_DEFAULT);
            icu::UnicodeString strPatternCopy = strPattern.m_strString;
            strPatternCopy.foldCase(U_FOLD_CASE_DEFAULT);
            nPosition = strResidentCopy.indexOf(strPatternCopy);
        }
        else
        {
            UErrorCode errorCode = U_ZERO_ERROR;

            // About string search with ICU: http://userguide.icu-project.org/collation/icu-string-search-service
            icu::StringSearch search(strPattern.m_strString, m_strString, Locale::getEnglish(), NULL, errorCode);

            QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when creating the internal search object");

            QStringUnicode::ConfigureSearch(eComparisonType, search);
            nPosition = search.next(errorCode);
            
            QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when searching the pattern");
        }

        if(nPosition == USEARCH_DONE)
            nPosition = QStringUnicode::PATTERN_NOT_FOUND;
    }

    return nPosition;
}

void QStringUnicode::ConfigureSearch(const EQComparisonType &eComparisonType, icu::StringSearch &search)
{
    UErrorCode errorCode = U_ZERO_ERROR;

    switch(eComparisonType)
    {
    case EQComparisonType::E_CanonicalCaseInsensitive:
        search.setAttribute(USEARCH_CANONICAL_MATCH, USEARCH_ON, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (USEARCH_CANONICAL_MATCH)");
        search.setAttribute(USEARCH_OVERLAP, USEARCH_OFF, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (USEARCH_OVERLAP)");
        search.getCollator()->setStrength(icu::Collator::SECONDARY); // Secondary: No tertiary checking, no case comparison
        search.getCollator()->setAttribute(UCOL_NORMALIZATION_MODE, UCOL_ON, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (UCOL_NORMALIZATION_MODE)");
        search.getCollator()->setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_NON_IGNORABLE, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (UCOL_ALTERNATE_HANDLING)");
        break;
    case EQComparisonType::E_CanonicalCaseSensitive:
        search.setAttribute(USEARCH_CANONICAL_MATCH, USEARCH_ON, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (USEARCH_CANONICAL_MATCH)");
        search.setAttribute(USEARCH_OVERLAP, USEARCH_OFF, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (USEARCH_OVERLAP)");
        search.getCollator()->setStrength(icu::Collator::TERTIARY); // Tertiary: Case comparison
        search.getCollator()->setAttribute(UCOL_NORMALIZATION_MODE, UCOL_ON, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (UCOL_NORMALIZATION_MODE)");
        search.getCollator()->setAttribute(UCOL_ALTERNATE_HANDLING, UCOL_NON_IGNORABLE, errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An error occurred when calling setAttribute (UCOL_NON_IGNORABLE)");
        break;
    case EQComparisonType::E_CompatibilityCaseInsensitive:
    case EQComparisonType::E_CompatibilityCaseSensitive:
        QE_ASSERT(false, "Compatibility comparisons are not supported yet");
        break;
    default:
        QE_ASSERT(false, "Invalid comparison type");
    }
}

int QStringUnicode::IndexOf(const QStringUnicode &strPattern, const EQComparisonType &eComparisonType, const unsigned int uStart) const
{
    int32_t nPosition = QStringUnicode::PATTERN_NOT_FOUND;

    if(!strPattern.IsEmpty() && !this->IsEmpty() && uStart < this->GetLength())
    {
        if(eComparisonType == EQComparisonType::E_BinaryCaseSensitive)
        {
            nPosition = m_strString.indexOf(strPattern.m_strString, scast_q(uStart, int32_t));
        }
        else if(eComparisonType == EQComparisonType::E_BinaryCaseInsensitive)
        {
            // There is no case insensitive indexOf in ICU so both strings are converted to lowercase before searching
            icu::UnicodeString strResidentCopy = m_strString;
            strResidentCopy.foldCase(U_FOLD_CASE_DEFAULT);
            icu::UnicodeString strPatternCopy = strPattern.m_strString;
            strPatternCopy.foldCase(U_FOLD_CASE_DEFAULT);
            nPosition = strResidentCopy.indexOf(strPatternCopy);
        }
        else
        {
            UErrorCode errorCode = U_ZERO_ERROR;

            // About string search with ICU: http://userguide.icu-project.org/collation/icu-string-search-service
            icu::StringSearch search(strPattern.m_strString, m_strString, Locale::getEnglish(), NULL, errorCode);
            QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when creating the internal search object");

            search.setOffset(scast_q(uStart, int32_t), errorCode);
            QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when setting the offset of the search");

            QStringUnicode::ConfigureSearch(eComparisonType, search);
            nPosition = search.next(errorCode);

            QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when searching the pattern");
        }

        if(nPosition == USEARCH_DONE)
            nPosition = QStringUnicode::PATTERN_NOT_FOUND;
    }

    return nPosition;
}

void QStringUnicode::Replace(const QStringUnicode &strSearchedPattern, const QStringUnicode &strReplacement, const EQComparisonType &eComparisonType)
{
    if(!(strSearchedPattern.IsEmpty() || this->IsEmpty()))
    {
        if(eComparisonType == EQComparisonType::E_BinaryCaseSensitive)
            m_strString.findAndReplace(strSearchedPattern.m_strString, strReplacement.m_strString);
        else if(eComparisonType == EQComparisonType::E_BinaryCaseInsensitive)
            this->ReplaceBinaryCaseInsensitive(strSearchedPattern, strReplacement);
        else
            this->ReplaceCanonical(strSearchedPattern, strReplacement, eComparisonType);
    }
}

void QStringUnicode::ReplaceBinaryCaseInsensitive(const QStringUnicode &strSearchedPattern, const QStringUnicode &strReplacement)
{
    icu::UnicodeString strPatternCopy = strSearchedPattern.m_strString;
    strPatternCopy.foldCase(U_FOLD_CASE_DEFAULT);
    icu::UnicodeString strResidentCopy = m_strString;
    strResidentCopy.foldCase(U_FOLD_CASE_DEFAULT);

    // Iterates over a copy of the original string, but modifying the original string
    int32_t nPosition = strResidentCopy.indexOf(strPatternCopy);
    int32_t nAccumulatedOffset = 0;
    const int32_t PATTERN_LENGTH = scast_q(strSearchedPattern.GetLength(), int32_t);
    const int32_t REPLACEMENT_LENGTH = scast_q(strReplacement.GetLength(), int32_t);
    const int32_t REPLACEMENT_LENGTH_DIFFERENCE = REPLACEMENT_LENGTH - PATTERN_LENGTH;

    while(nPosition != USEARCH_DONE)
    {
        m_strString.replace(nPosition + nAccumulatedOffset, PATTERN_LENGTH, strReplacement.m_strString);

        // An offset must be kept because the string being modified and the string the iterator traverses are not the same instance
        // Adding this offset we get the position of the found match in the modified string
        nAccumulatedOffset += REPLACEMENT_LENGTH_DIFFERENCE;

        nPosition = strResidentCopy.indexOf(strPatternCopy, nPosition + 1);
    }
}

void QStringUnicode::ReplaceCanonical(const QStringUnicode& strSearchedPattern, const QStringUnicode &strReplacement, const EQComparisonType &eComparisonType)
{
    // Creates the search object
    UErrorCode errorCode = U_ZERO_ERROR;
    icu::StringSearch search(strSearchedPattern.m_strString, m_strString, Locale::getEnglish(), NULL, errorCode);
    QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when creating the internal search object");

    QStringUnicode::ConfigureSearch(eComparisonType, search);

    // Iterates over a copy of the original string, modifying the original string
    int32_t nPosition = search.next(errorCode);
    int32_t nAccumulatedOffset = 0;
    const int32_t PATTERN_LENGTH = scast_q(strSearchedPattern.GetLength(), int32_t);
    const int32_t REPLACEMENT_LENGTH = scast_q(strReplacement.GetLength(), int32_t);
    const int32_t REPLACEMENT_LENGTH_DIFFERENCE = REPLACEMENT_LENGTH - PATTERN_LENGTH;

    QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when searching the pattern");

    while(nPosition != USEARCH_DONE)
    {
        m_strString.replace(nPosition + nAccumulatedOffset, PATTERN_LENGTH, strReplacement.m_strString);

        // An offset must be kept because the string being modified and the string the iterator traverses are not the same instance
        // Adding this offset we get the position of the found match in the modified string
        nAccumulatedOffset += REPLACEMENT_LENGTH_DIFFERENCE;

        nPosition = search.next(errorCode);
        QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error occurred when searching the pattern");
    }
}

void QStringUnicode::Append(const QStringUnicode &strStringToAppend)
{
    m_strString.append(strStringToAppend.m_strString);
}

i64_q QStringUnicode::ToInteger() const
{
    const icu::NumberFormat* pFormatter = QStringUnicode::GetIntegerFormatter();
    UErrorCode errorCodeParsing = U_ZERO_ERROR;
    UErrorCode errorCodeGetting = U_ZERO_ERROR;

    icu::Formattable formattable;
    pFormatter->parse(m_strString, formattable, errorCodeParsing);
    
    i64_q nResult = formattable.getInt64(errorCodeGetting);

    QE_ASSERT(U_SUCCESS(errorCodeParsing) || nResult != 0LL, "The string cannot be converted to an integer number");
    QE_ASSERT(U_SUCCESS(errorCodeParsing) || nResult == 0LL, "The number contained in the string is too big, it will be shrinked to the maximum value the type can represent");
    QE_ASSERT(U_SUCCESS(errorCodeGetting), "An unexpected error occurred when getting the value from the formattable object");

    return nResult;
}

bool  QStringUnicode::ToBoolean() const
{
    static const QStringUnicode BOOLEAN_STRING_TRUE("true");
    static const QStringUnicode BOOLEAN_STRING_TRUE_NUMERIC("1");
    static const QStringUnicode BOOLEAN_STRING_TRUE_ABBREVIATED1("t");
    static const QStringUnicode BOOLEAN_STRING_TRUE_ABBREVIATED2("T");
    static const QStringUnicode BOOLEAN_STRING_TRUE_ABBREVIATED3("y");
    static const QStringUnicode BOOLEAN_STRING_TRUE_ABBREVIATED4("Y");
    static const QStringUnicode BOOLEAN_STRING_FALSE("false");
    static const QStringUnicode BOOLEAN_STRING_FALSE_NUMERIC("0");
    static const QStringUnicode BOOLEAN_STRING_FALSE_ABBREVIATED1("f");
    static const QStringUnicode BOOLEAN_STRING_FALSE_ABBREVIATED2("F");
    static const QStringUnicode BOOLEAN_STRING_FALSE_ABBREVIATED3("n");
    static const QStringUnicode BOOLEAN_STRING_FALSE_ABBREVIATED4("N");

    bool bResult = false;

    if(this->GetLength() > 1)
    {
        bResult = this->CompareTo(BOOLEAN_STRING_TRUE, EQComparisonType::E_BinaryCaseInsensitive) == 0;
        
        // If it's false and the string does not match the "false" word, then it has not got a valid value
        QE_ASSERT(bResult == true || (this->CompareTo(BOOLEAN_STRING_FALSE, EQComparisonType::E_BinaryCaseInsensitive) == 0), "The string cannot be converted to a boolean value");
    }
    else
    {
        bResult = *this == BOOLEAN_STRING_TRUE_NUMERIC      || 
                  *this == BOOLEAN_STRING_TRUE_ABBREVIATED1 || 
                  *this == BOOLEAN_STRING_TRUE_ABBREVIATED2 || 
                  *this == BOOLEAN_STRING_TRUE_ABBREVIATED3 || 
                  *this == BOOLEAN_STRING_TRUE_ABBREVIATED4;

        // If it's false and the string does not match any of the abbreviations, then it has not got a valid value
        QE_ASSERT(bResult == true                            || 
                  *this == BOOLEAN_STRING_FALSE_NUMERIC      ||                   *this == BOOLEAN_STRING_FALSE_ABBREVIATED1 || 
                  *this == BOOLEAN_STRING_FALSE_ABBREVIATED2 ||
                  *this == BOOLEAN_STRING_FALSE_ABBREVIATED3 ||
                  *this == BOOLEAN_STRING_FALSE_ABBREVIATED4, 
                  "The string cannot be converted to a boolean value");
    }

    return bResult;
}

f64_q QStringUnicode::ToFloat() const
{
#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT != QE_CONFIG_ASSERTSBEHAVIOR_DISABLED
    static const f64_q POSITIVE_INFINITE = +1e308 * 1e308;
    static const f64_q NEGATIVE_INFINITE = -1e308 * 1e308;
#endif
    const icu::NumberFormat* pFormatter = QStringUnicode::GetFloatFormatter();
    UErrorCode errorCodeParsing = U_ZERO_ERROR;
    UErrorCode errorCodeGetting = U_ZERO_ERROR;

    icu::Formattable formattable;
    pFormatter->parse(m_strString, formattable, errorCodeParsing);

    f64_q fResult = formattable.getDouble(errorCodeGetting);

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT != QE_CONFIG_ASSERTSBEHAVIOR_DISABLED
    QE_ASSERT(fResult != POSITIVE_INFINITE && fResult != NEGATIVE_INFINITE, "The result is infinite, maybe the string contains a too big number");
#endif
    QE_ASSERT(U_SUCCESS(errorCodeParsing) || fResult != 0.0, "The string cannot be converted to a floating point number");
    QE_ASSERT(U_SUCCESS(errorCodeParsing) || fResult == 0.0, "The number contained in the string is too big, it will be shrinked to the maximum value the type can represent");
    QE_ASSERT(U_SUCCESS(errorCodeGetting), "An unexpected error occurred when getting the value from the formattable object");

    return fResult;
}

const icu::NumberFormat* QStringUnicode::GetIntegerFormatter()
{
    static UErrorCode errorCode = U_ZERO_ERROR;
    static icu::NumberFormat* pNumberFormatter = icu::NumberFormat::createInstance(icu::Locale::getEnglish(), UNUM_DECIMAL, errorCode);
    pNumberFormatter->setParseIntegerOnly(TRUE);
    QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error ocurred when creating an ICU number formatter");
    return pNumberFormatter;
}

const icu::NumberFormat* QStringUnicode::GetFloatFormatter()
{
    static UErrorCode errorCode = U_ZERO_ERROR;
    static const icu::NumberFormat* pNumberFormatter = icu::DecimalFormat::createInstance(icu::Locale::getEnglish(), UNUM_SCIENTIFIC, errorCode);
    QE_ASSERT(U_SUCCESS(errorCode), "An unexpected error ocurred when creating an ICU number formatter");
    return pNumberFormatter;
}



//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |         PROPERTIES		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

unsigned int QStringUnicode::GetLength() const
{
    return scast_q(m_strString.countChar32(), unsigned int);
}

bool QStringUnicode::IsEmpty() const
{
    return m_strString.isEmpty() != FALSE;
}

const QStringUnicode& QStringUnicode::GetEmpty()
{
    static const QStringUnicode EMPTY_STRING;
    return EMPTY_STRING;
}


} //namespace DataTypes
} //namespace Common
} //namespace QuimeraEngine
} //namespace Kinesis
