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

#include "QDateTime.h"

#include "Assertions.h"
#include "SQInteger.h"
#include "QTimeZone.h"

using Kinesis::QuimeraEngine::Common::DataTypes::SQInteger;


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Time
{

//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |  ATTRIBUTES INITIALIZATION |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

const u64_q QDateTime::HALF_VALUE = 9223372036854775808ULL;
const u64_q QDateTime::HNS_PER_MICROSECOND = 10ULL;
const u64_q QDateTime::HNS_PER_MILLISECOND = 10000ULL;
const u64_q QDateTime::HNS_PER_SECOND   = 10000000ULL;
const u64_q QDateTime::HNS_PER_MINUTE   = 600000000ULL;
const u64_q QDateTime::HNS_PER_HOUR     = 36000000000ULL;
const u64_q QDateTime::HNS_PER_DAY      = 864000000000ULL;
const u64_q QDateTime::HNS_PER_YEAR     = 315360000000000ULL;
const u64_q QDateTime::HNS_PER_LEAPYEAR = 316224000000000ULL;
const u64_q QDateTime::HNS_PER_4_CONSECUTIVE_YEARS = 1262304000000000ULL;
const QTimeSpan QDateTime::UNDEFINED_VALUE(0ULL);


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |       CONSTRUCTORS		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

QDateTime::QDateTime() : m_instant(QTimeSpan(0)),
                         m_pTimeZone(null_q)
{
}

QDateTime::QDateTime(const QDateTime &dateTime) : m_instant(dateTime.m_instant),
                                                  m_pTimeZone(dateTime.m_pTimeZone)
{
}

QDateTime::QDateTime(const i32_q nYear, const u64_q uMonth, const u64_q uDay,
                     const u64_q uHour, const u64_q uMinute, const u64_q uSecond,
                     const u64_q uMillisecond, const u64_q uMicrosecond, const u64_q uHundredsOfNanosecond,
                     const QTimeZone* pTimeZone) : m_pTimeZone(pTimeZone)
{
    // Adjustment required to store the day 1 (Gregorian calendar) as day zero (decimal system)
    static const u64_q DAY_ZERO_ADJUSTMENT = 1ULL;

    // How many years pass among leap years
    static const u64_q LEAP_YEAR_FREQUENCY = 4ULL;

    // These constants represent the maximum date (+29228-02-08 02:48:05.477.580.7) and the minimum date (-29228-11-23 21:11:54.522.419.3)
    static const i32_q MAX_DATE_YEAR        = 29228;
    static const u64_q MAX_DATE_MONTH       = 2ULL;
    static const u64_q MAX_DATE_DAY         = 8ULL;
    static const u64_q MAX_DATE_HOUR        = 2ULL;
    static const u64_q MAX_DATE_MINUTE      = 48ULL;
    static const u64_q MAX_DATE_SECOND      = 5ULL;
    static const u64_q MAX_DATE_MILLISECOND = 477ULL;
    static const u64_q MAX_DATE_MICROSECOND = 580ULL;
    static const u64_q MAX_DATE_HNS         = 7ULL;
    static const i32_q MIN_DATE_YEAR        = -29228;
    static const u64_q MIN_DATE_MONTH       = 11ULL;
    static const u64_q MIN_DATE_DAY         = 23ULL;
    static const u64_q MIN_DATE_HOUR        = 21ULL;
    static const u64_q MIN_DATE_MINUTE      = 11ULL;
    static const u64_q MIN_DATE_SECOND      = 54ULL;
    static const u64_q MIN_DATE_MILLISECOND = 522ULL;
    static const u64_q MIN_DATE_MICROSECOND = 419ULL;
    static const u64_q MIN_DATE_HNS         = 3ULL;

    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_MONTH       = 12ULL;
    static const u64_q MAX_DAY         = 31ULL;
    static const u64_q MAX_HOUR        = 23ULL;
    static const u64_q MAX_MINUTE      = 59ULL;
    static const u64_q MAX_SECOND      = 59ULL;
    static const u64_q MAX_MILLISECOND = 999ULL;
    static const u64_q MAX_MICROSECOND = 999ULL;
    static const u64_q MAX_HNS         = 9ULL;

    // Checks for invalid inputs
    QE_ASSERT(nYear != 0, "The year zero does not exist");
    QE_ASSERT(uMonth <= MAX_MONTH, "The value of the month is greater than the maximum allowed, it must be in the range [1, 12]");
    QE_ASSERT(uMonth != 0, "The value of the month lower than the minimum allowed, it must be in the range [1, 12]");
    QE_ASSERT(uDay <= MAX_DAY, "The value of the day is greater than the maximum allowed, it must be in the range [1, 31]");
    QE_ASSERT(uDay != 0, "The value of the day is lower than the minimum allowed, it must be in the range [1, 31]");
    QE_ASSERT(uDay <= QDateTime::GetDaysInMonth(scast_q(uMonth, unsigned int), nYear), "The value of the day is not valid for the provided month in the provided year");
    QE_ASSERT(uHour <= MAX_HOUR, "The value of the hour is not valid, it must be in the range [0, 23]");
    QE_ASSERT(uMinute <= MAX_MINUTE, "The value of the minute is not valid, it must be in the range [0, 59]");
    QE_ASSERT(uSecond <= MAX_SECOND, "The value of the second is not valid, it must be in the range [0, 59]");
    QE_ASSERT(uMillisecond <= MAX_MILLISECOND, "The value of the milliseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT(uMicrosecond <= MAX_MICROSECOND, "The value of the microseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT(uHundredsOfNanosecond <= MAX_HNS, "The value of the hundreds of nanoseconds is not valid, it must be in the range [0, 9]");

    // Checks for representable limits
    const bool INPUT_IS_OUT_OF_POSITIVE_LIMIT = nYear > MAX_DATE_YEAR                                                         ||
                                                (nYear   ==      MAX_DATE_YEAR   && (uMonth       > MAX_DATE_MONTH            ||
                                                (uMonth  ==      MAX_DATE_MONTH  && (uDay         > MAX_DATE_DAY              ||
                                                (uDay    ==      MAX_DATE_DAY    && (uHour        > MAX_DATE_HOUR             ||
                                                (uHour   ==      MAX_DATE_HOUR   && (uMinute      > MAX_DATE_MINUTE           ||
                                                (uMinute ==      MAX_DATE_MINUTE && (uSecond      > MAX_DATE_SECOND           ||
                                                (uSecond ==      MAX_DATE_SECOND && (uMillisecond > MAX_DATE_MILLISECOND      ||
                                                (uMillisecond == MAX_DATE_MILLISECOND && (uMicrosecond > MAX_DATE_MICROSECOND ||
                                                (uMicrosecond == MAX_DATE_MICROSECOND && (uHundredsOfNanosecond > MAX_DATE_HNS))))))))))))))));

    const bool INPUT_IS_OUT_OF_NEGATIVE_LIMIT = nYear < MIN_DATE_YEAR                                                         ||
                                                (nYear   ==      MIN_DATE_YEAR   && (uMonth       < MIN_DATE_MONTH            ||
                                                (uMonth  ==      MIN_DATE_MONTH  && (uDay         < MIN_DATE_DAY              ||
                                                (uDay    ==      MIN_DATE_DAY    && (uHour        < MIN_DATE_HOUR             ||
                                                (uHour   ==      MIN_DATE_HOUR   && (uMinute      < MIN_DATE_MINUTE           ||
                                                (uMinute ==      MIN_DATE_MINUTE && (uSecond      < MIN_DATE_SECOND           ||
                                                (uSecond ==      MIN_DATE_SECOND && (uMillisecond < MIN_DATE_MILLISECOND      ||
                                                (uMillisecond == MIN_DATE_MILLISECOND && (uMicrosecond < MIN_DATE_MICROSECOND ||
                                                (uMicrosecond == MIN_DATE_MICROSECOND && (uHundredsOfNanosecond < MIN_DATE_HNS))))))))))))))));

    QE_ASSERT(!INPUT_IS_OUT_OF_POSITIVE_LIMIT, "The input date and time is posterior to the maximum value that can be represented by this type");
    QE_ASSERT(!INPUT_IS_OUT_OF_NEGATIVE_LIMIT, "The input date and time is anterior to the minimum value that can be represented by this type");

    if(INPUT_IS_OUT_OF_POSITIVE_LIMIT)
    {
        // If the input date and time exceed the highest limit, the result will be the maximum value
        m_instant = QTimeSpan(SQInteger::MaxU64_Q);
    }
    else if(INPUT_IS_OUT_OF_NEGATIVE_LIMIT)
    {
        // If the input date and time exceed the lowest limit, the result will be the minimum value
        m_instant = QTimeSpan(1ULL);
    }
    else
    {
        // All the input values are converted to hundreds of nanoseconds (hns)
        const u64_q MICROSECOND_AS_HNS = uMicrosecond * QDateTime::HNS_PER_MICROSECOND;
        const u64_q MILLISECOND_AS_HNS = uMillisecond * QDateTime::HNS_PER_MILLISECOND;
        const u64_q SECOND_AS_HNS = uSecond * QDateTime::HNS_PER_SECOND;
        const u64_q MINUTE_AS_HNS = uMinute * QDateTime::HNS_PER_MINUTE;
        const u64_q HOUR_AS_HNS = uHour * QDateTime::HNS_PER_HOUR;
        const u64_q DAY_AS_HNS = (uDay - DAY_ZERO_ADJUSTMENT) * QDateTime::HNS_PER_DAY;

        // Sums the days of all completed months
        u64_q uDaysInCompleteMonths = 0;

        for(unsigned int i = 1U; i < uMonth; ++i)
            uDaysInCompleteMonths += QDateTime::GetDaysInMonth(i, nYear);

        const u64_q MONTH_AS_HNS = uDaysInCompleteMonths * QDateTime::HNS_PER_DAY;

        const u64_q ABS_YEAR = scast_q(SQInteger::Abs(nYear), u64_q);
        const bool IS_LEAP_YEAR = QDateTime::IsLeapYear(nYear);

        // Hundreds of nanoseconds in passed years: abs(T.YEAR) / 4 * nsh in 4 consecutive years + (abs(T.YEAR) % 4) * nsh in a year
        u64_q uYearsAsHns = ABS_YEAR / LEAP_YEAR_FREQUENCY * QDateTime::HNS_PER_4_CONSECUTIVE_YEARS + (ABS_YEAR % LEAP_YEAR_FREQUENCY) * QDateTime::HNS_PER_YEAR;

        // Offset applied to positive dates in order to make the year zero-based
        if(nYear > 0)
            uYearsAsHns -= IS_LEAP_YEAR ? HNS_PER_LEAPYEAR : HNS_PER_YEAR;

        // First, the last year's remaining time is summed
        const u64_q REMAINING_TIME = MONTH_AS_HNS + DAY_AS_HNS + HOUR_AS_HNS + MINUTE_AS_HNS + SECOND_AS_HNS +
                                     MILLISECOND_AS_HNS + MICROSECOND_AS_HNS + uHundredsOfNanosecond;

        QTimeSpan instant;

        if(nYear < 0)
            // If the year is negative, years are subtracted to the offset and days and time are added
            instant = QTimeSpan(QDateTime::HALF_VALUE - uYearsAsHns + REMAINING_TIME);
        else
            // If the year is positive, years, days and time are added to the offset
            instant = QTimeSpan(QDateTime::HALF_VALUE + uYearsAsHns + REMAINING_TIME);

        // Removes the time zone offset
        m_instant = pTimeZone == null_q ? instant :
                                          this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
    }
}

QDateTime::QDateTime(const i32_q nYear, const u64_q uMonth, const u64_q uDay, const QTimeZone* pTimeZone) :
                m_pTimeZone(pTimeZone)
{
    // Adjustment required to store the day 1 (Gregorian calendar) as day zero (decimal system)
    static const u64_q DAY_ZERO_ADJUSTMENT = 1ULL;

    // How many years pass among leap years
    static const u64_q LEAP_YEAR_FREQUENCY = 4ULL;

    // These constants represent the maximum date (+29228-02-08 02:48:05.477.580.7) and the minimum date (-29228-11-23 21:11:54.522.419.3)
    static const i32_q MAX_DATE_YEAR        = 29228;
    static const u64_q MAX_DATE_MONTH       = 2ULL;
    static const u64_q MAX_DATE_DAY         = 8ULL;
    static const i32_q MIN_DATE_YEAR        = -29228;
    static const u64_q MIN_DATE_MONTH       = 11ULL;
    static const u64_q MIN_DATE_DAY         = 24ULL; // IMPORTANT: It cannot be 23 because the time would be 00:00:00, which is anterior to 21:11:54

    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_MONTH       = 12ULL;
    static const u64_q MAX_DAY         = 31ULL;

    // Checks for invalid inputs
    QE_ASSERT(nYear != 0, "The year zero does not exist");
    QE_ASSERT(uMonth <= MAX_MONTH, "The value of the month is greater than the maximum allowed, it must be in the range [1, 12]");
    QE_ASSERT(uMonth != 0, "The value of the month lower than the minimum allowed, it must be in the range [1, 12]");
    QE_ASSERT(uDay <= MAX_DAY, "The value of the day is greater than the maximum allowed, it must be in the range [1, 31]");
    QE_ASSERT(uDay != 0, "The value of the day is lower than the minimum allowed, it must be in the range [1, 31]");
    QE_ASSERT(uDay <= QDateTime::GetDaysInMonth(scast_q(uMonth, unsigned int), nYear), "The value of the day is not valid for the provided month in the provided year");

    // Checks for representable limits
    const bool INPUT_IS_OUT_OF_POSITIVE_LIMIT = nYear > MAX_DATE_YEAR                                                         ||
                                                (nYear   ==      MAX_DATE_YEAR   && (uMonth       > MAX_DATE_MONTH            ||
                                                (uMonth  ==      MAX_DATE_MONTH  && (uDay         > MAX_DATE_DAY))));

    const bool INPUT_IS_OUT_OF_NEGATIVE_LIMIT = nYear < MIN_DATE_YEAR                                                         ||
                                                (nYear   ==      MIN_DATE_YEAR   && (uMonth       < MIN_DATE_MONTH            ||
                                                (uMonth  ==      MIN_DATE_MONTH  && (uDay         < MIN_DATE_DAY))));

    QE_ASSERT(!INPUT_IS_OUT_OF_POSITIVE_LIMIT, "The input date and time is posterior to the maximum value that can be represented by this type");
    QE_ASSERT(!INPUT_IS_OUT_OF_NEGATIVE_LIMIT, "The input date and time is anterior to the minimum value that can be represented by this type");

    if(INPUT_IS_OUT_OF_POSITIVE_LIMIT)
    {
        // If the input date and time exceed the highest limit, the result will be the maximum value
        m_instant = QTimeSpan(SQInteger::MaxU64_Q);
    }
    else if(INPUT_IS_OUT_OF_NEGATIVE_LIMIT)
    {
        // If the input date and time exceed the lowest limit, the result will be the minimum value
        m_instant = QTimeSpan(1ULL);
    }
    else
    {
        // All the input values are converted to hundreds of nanoseconds (hns)
        const u64_q DAY_AS_HNS = (uDay - DAY_ZERO_ADJUSTMENT) * QDateTime::HNS_PER_DAY;

        // Sums the days of all completed months
        u64_q uDaysInCompleteMonths = 0;

        for(unsigned int i = 1U; i < uMonth; ++i)
            uDaysInCompleteMonths += QDateTime::GetDaysInMonth(i, nYear);

        const u64_q MONTH_AS_HNS = uDaysInCompleteMonths * QDateTime::HNS_PER_DAY;

        const u64_q ABS_YEAR = scast_q(SQInteger::Abs(nYear), u64_q);
        const bool IS_LEAP_YEAR = QDateTime::IsLeapYear(nYear);

        // Hundreds of nanoseconds in passed years: abs(T.YEAR) / 4 * nsh in 4 consecutive years + (abs(T.YEAR) % 4) * nsh in a year
        u64_q uYearsAsHns = ABS_YEAR / LEAP_YEAR_FREQUENCY * QDateTime::HNS_PER_4_CONSECUTIVE_YEARS + (ABS_YEAR % LEAP_YEAR_FREQUENCY) * QDateTime::HNS_PER_YEAR;

        // Offset applied to positive dates in order to make the year zero-based
        if(nYear > 0)
            uYearsAsHns -= IS_LEAP_YEAR ? QDateTime::HNS_PER_LEAPYEAR : QDateTime::HNS_PER_YEAR;

        // First, the last year's remaining time is summed
        const u64_q REMAINING_TIME = MONTH_AS_HNS + DAY_AS_HNS;

        QTimeSpan instant;

        if(nYear < 0)
            // If the year is negative, years are subtracted to the offset and days and time are added
            instant = QTimeSpan(QDateTime::HALF_VALUE - uYearsAsHns + REMAINING_TIME);
        else
            // If the year is positive, years, days and time are added to the offset
            instant = QTimeSpan(QDateTime::HALF_VALUE + uYearsAsHns + REMAINING_TIME);

        // Removes the time zone offset
        m_instant = pTimeZone == null_q ? instant :
                                          this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
    }
}

QDateTime::QDateTime(const u64_q uHour, const u64_q uMinute, const u64_q uSecond,
                     const u64_q uMillisecond, const u64_q uMicrosecond, const u64_q uHundredsOfNanosecond,
                     const QTimeZone* pTimeZone) : m_pTimeZone(pTimeZone)
{
    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_HOUR        = 23ULL;
    static const u64_q MAX_MINUTE      = 59ULL;
    static const u64_q MAX_SECOND      = 59ULL;
    static const u64_q MAX_MILLISECOND = 999ULL;
    static const u64_q MAX_MICROSECOND = 999ULL;
    static const u64_q MAX_HNS         = 9ULL;

    // Checks for invalid inputs
    QE_ASSERT(uHour <= MAX_HOUR, "The value of the hour is not valid, it must be in the range [0, 23]");
    QE_ASSERT(uMinute <= MAX_MINUTE, "The value of the minute is not valid, it must be in the range [0, 59]");
    QE_ASSERT(uSecond <= MAX_SECOND, "The value of the second is not valid, it must be in the range [0, 59]");
    QE_ASSERT(uMillisecond <= MAX_MILLISECOND, "The value of the milliseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT(uMicrosecond <= MAX_MICROSECOND, "The value of the microseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT(uHundredsOfNanosecond <= MAX_HNS, "The value of the hundreds of nanoseconds is not valid, it must be in the range [0, 9]");


    // All the input values are converted to hundreds of nanoseconds (hns)
    const u64_q MICROSECOND_AS_HNS = uMicrosecond * QDateTime::HNS_PER_MICROSECOND;
    const u64_q MILLISECOND_AS_HNS = uMillisecond * QDateTime::HNS_PER_MILLISECOND;
    const u64_q SECOND_AS_HNS = uSecond * QDateTime::HNS_PER_SECOND;
    const u64_q MINUTE_AS_HNS = uMinute * QDateTime::HNS_PER_MINUTE;
    const u64_q HOUR_AS_HNS = uHour * QDateTime::HNS_PER_HOUR;

    // First, the last year's remaining time is summed
    const u64_q REMAINING_TIME = HOUR_AS_HNS + MINUTE_AS_HNS + SECOND_AS_HNS +
                                 MILLISECOND_AS_HNS + MICROSECOND_AS_HNS + uHundredsOfNanosecond;

    QTimeSpan instant = QTimeSpan(QDateTime::HALF_VALUE + REMAINING_TIME);

    // Removes the time zone offset
    m_instant = pTimeZone == null_q ? instant :
                                      this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
}

QDateTime::QDateTime(const u64_q uHour, const u64_q uMinute, const u64_q uSecond, const u64_q uMillisecond, const QTimeZone* pTimeZone) :
                m_pTimeZone(pTimeZone)
{
    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_HOUR        = 23ULL;
    static const u64_q MAX_MINUTE      = 59ULL;
    static const u64_q MAX_SECOND      = 59ULL;
    static const u64_q MAX_MILLISECOND = 999ULL;

    // Checks for invalid inputs
    QE_ASSERT(uHour <= MAX_HOUR, "The value of the hour is not valid, it must be in the range [0, 23]");
    QE_ASSERT(uMinute <= MAX_MINUTE, "The value of the minute is not valid, it must be in the range [0, 59]");
    QE_ASSERT(uSecond <= MAX_SECOND, "The value of the second is not valid, it must be in the range [0, 59]");
    QE_ASSERT(uMillisecond <= MAX_MILLISECOND, "The value of the milliseconds is not valid, it must be in the range [0, 999]");

    // All the input values are converted to hundreds of nanoseconds (hns)
    const u64_q MILLISECOND_AS_HNS = uMillisecond * QDateTime::HNS_PER_MILLISECOND;
    const u64_q SECOND_AS_HNS = uSecond * QDateTime::HNS_PER_SECOND;
    const u64_q MINUTE_AS_HNS = uMinute * QDateTime::HNS_PER_MINUTE;
    const u64_q HOUR_AS_HNS = uHour * QDateTime::HNS_PER_HOUR;

    // First, the last year's remaining time is summed
    const u64_q REMAINING_TIME = HOUR_AS_HNS + MINUTE_AS_HNS + SECOND_AS_HNS + MILLISECOND_AS_HNS;

    // If the year is positive, years, days and time are added to the offset
    QTimeSpan instant = QTimeSpan(QDateTime::HALF_VALUE + REMAINING_TIME);

    // Removes the time zone offset
    m_instant = pTimeZone == null_q ? instant :
                                      this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
}

QDateTime::QDateTime(const QDateTime &dateTime, const QTimeZone* pTimeZone) : m_instant(dateTime.m_instant),
                                                                              m_pTimeZone(pTimeZone)
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

QDateTime& QDateTime::operator=(const QDateTime& dateTime)
{
    if(this != &dateTime)
    {
        this->m_instant = dateTime.m_instant;
        this->m_pTimeZone = dateTime.m_pTimeZone;
    }

    return *this;
}

QDateTime& QDateTime::operator+=(const QTimeSpan &timeToAdd)
{
    QE_ASSERT(!this->IsUndefined(), "The date/time is undefined, the time span cannot be added");

    QE_ASSERT(QDateTime::GetMaxDateTime().m_instant - m_instant >= timeToAdd, "The result of adding that time span exceeds the maximum date, the result will be set to the maximum date allowed");

    // Note: It assumes that the QTimeSpan class already prevents from overflow
    if(*this != QDateTime::GetUndefinedDate())
        m_instant += timeToAdd;

    return *this;
}

QDateTime& QDateTime::operator-=(const QTimeSpan &timeToSubtract)
{
    QE_ASSERT(!this->IsUndefined(), "The date/time is undefined, the time span cannot be subtracted");

    if(*this != QDateTime::GetUndefinedDate())
    {
        QE_ASSERT(m_instant > timeToSubtract, "The result of subtracting that time span exceeds the minimum date, the result will be set to the minimum date allowed");

        if(m_instant <= timeToSubtract)
            m_instant = QDateTime::GetMinDateTime().m_instant;
        else
            m_instant -= timeToSubtract;
    }

    return *this;
}

QDateTime QDateTime::operator+(const QTimeSpan &timeToAdd) const
{
    QDateTime result(*this);
    result += timeToAdd;

    return result;
}

QDateTime QDateTime::operator-(const QTimeSpan &timeToSubtract) const
{
    QDateTime result(*this);
    result -= timeToSubtract;

    return result;
}

QTimeSpan QDateTime::operator-(const QDateTime &dateTime) const
{
    QE_ASSERT(!this->IsUndefined() && !dateTime.IsUndefined(), "It is not possible to calculate the difference between undefined dates");

    if(this->IsUndefined() || dateTime.IsUndefined())
    {
        return QTimeSpan(0);
    }
    else
    {
        if(dateTime.m_instant < m_instant)
            return m_instant - dateTime.m_instant;
        else
            return dateTime.m_instant - m_instant;
    }
}

bool QDateTime::operator==(const QDateTime &dateTime) const
{
    // Other members are not checked
    return dateTime.m_instant == m_instant;
}

bool QDateTime::operator!=(const QDateTime &dateTime) const
{
    // Other members are not checked
    return dateTime.m_instant != m_instant;
}

bool QDateTime::operator<(const QDateTime &dateTime) const
{
    QE_ASSERT(!this->IsUndefined() && !dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return this->IsUndefined() || dateTime.IsUndefined() ? 
                false :
                m_instant < dateTime.m_instant;
}

bool QDateTime::operator>(const QDateTime &dateTime) const
{
    QE_ASSERT(!this->IsUndefined() && !dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return this->IsUndefined() || dateTime.IsUndefined() ? 
                false : 
                m_instant > dateTime.m_instant;
}

bool QDateTime::operator<=(const QDateTime &dateTime) const
{
    QE_ASSERT(this->IsUndefined() == dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return (!this->IsUndefined() && dateTime.IsUndefined()) || (this->IsUndefined() && !dateTime.IsUndefined()) ? // If only one operand is undefined
                false :
                m_instant <= dateTime.m_instant;
}

bool QDateTime::operator>=(const QDateTime &dateTime) const
{
    QE_ASSERT(this->IsUndefined() == dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return (!this->IsUndefined() && dateTime.IsUndefined()) || (this->IsUndefined() && !dateTime.IsUndefined()) ? // If only one operand is undefined
               false :
               m_instant >= dateTime.m_instant;
}

unsigned int QDateTime::GetDaysInMonth(const unsigned int uMonth, const int nYear)
{
    static const unsigned int FEBRUARY_INDEX = 2;
    static const unsigned int MAXIMUM_MONTH_INDEX = 12;
    static const unsigned int DAYS_IN_MONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    QE_ASSERT(uMonth != 0, "The month index must be greater than 0");
    QE_ASSERT(uMonth <= MAXIMUM_MONTH_INDEX, "The month index must be lower than 13");

    unsigned int uDays = 0;

    if(uMonth <= MAXIMUM_MONTH_INDEX)
        uDays = DAYS_IN_MONTH[uMonth - 1];

    // If the year is a leap year, add 1 to the number of days in February
    if(uMonth == FEBRUARY_INDEX && QDateTime::IsLeapYear(nYear))
        ++uDays;

    return uDays;
}

const QDateTime& QDateTime::GetUndefinedDate()
{
    static const QDateTime UNDEFINED_DATETIME;
    return UNDEFINED_DATETIME;
}

bool QDateTime::IsLeapYear(const int nYear)
{
    return SQInteger::Abs(nYear) % 4ULL == 0;
}

QTimeSpan QDateTime::GetInstantWithSubtractedTimeZoneOffset(const QTimeSpan &instant, const QTimeZone* pTimeZone) const
{
    QDateTime localDateTime;
    localDateTime.m_instant = instant;

    QTimeSpan timeZoneOffset(0);
    bool bOffsetIsNegative = false;
    pTimeZone->CalculateOffset(localDateTime, timeZoneOffset, bOffsetIsNegative);
        
    // The offset is subtracted to the calculated instant to make it UTC
    if(bOffsetIsNegative)
        localDateTime.m_instant += timeZoneOffset;
    else
        localDateTime.m_instant -= timeZoneOffset;

    return localDateTime.m_instant;
}

QTimeSpan QDateTime::GetInstantWithAddedTimeZoneOffset(const QTimeSpan &instant, const QTimeZone* pTimeZone) const
{
    QDateTime localDateTime;
    localDateTime.m_instant = instant;

    QTimeSpan timeZoneOffset(0);
    bool bOffsetIsNegative = false;
    pTimeZone->CalculateOffset(localDateTime, timeZoneOffset, bOffsetIsNegative);
        
    // The offset is added to the calculated instant to make it UTC
    if(bOffsetIsNegative)
        localDateTime.m_instant -= timeZoneOffset;
    else
        localDateTime.m_instant += timeZoneOffset;

    return localDateTime.m_instant;
}


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |         PROPERTIES		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

bool QDateTime::IsLeapYear() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent either normal years or leap years");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);
    
    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    // Read the technical documentation to know why it is 2 the value to compare to
    return REMAINING_YEARS > 2ULL;
}

const QTimeZone* QDateTime::GetTimeZone() const
{
    return m_pTimeZone;
}

unsigned int QDateTime::GetYear() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent years");
    
    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);
    u64_q uYear = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear != 0 && IS_NEGATIVE_DATE)
        ++uYear;

    return scast_q(uYear, unsigned int);
}

unsigned int QDateTime::GetMonth() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent months");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMonth = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uMonth = 1;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const u32_q FIRST_MONTH_OF_THE_YEAR = 1;
        static const u32_q LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        uMonth = uMonthCounter;
    }

    return scast_q(uMonth, unsigned int);
}

unsigned int QDateTime::GetDay() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent days");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uDay = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uDay = 1;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const u32_q FIRST_MONTH_OF_THE_YEAR = 1;
        static const u32_q LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        uDay = uDaysInLastYear + 1;
    }

    return scast_q(uDay, unsigned int);
}

unsigned int QDateTime::GetHour() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent hours");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uHour = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uHour = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        uHour = uHoursInLastDay / HNS_PER_HOUR;
    }

    return scast_q(uHour, unsigned int);
}

unsigned int QDateTime::GetMinute() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent minutes");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMinute = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uMinute = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q minutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        uMinute = minutesInLastHour / HNS_PER_MINUTE;
    }

    return scast_q(uMinute, unsigned int);
}

unsigned int QDateTime::GetSecond() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent seconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uSecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uSecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q minutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q secondsInLastMinute = minutesInLastHour % HNS_PER_MINUTE;
        uSecond = secondsInLastMinute / HNS_PER_SECOND;
    }

    return scast_q(uSecond, unsigned int);
}

unsigned int QDateTime::GetMillisecond() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent milliseconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMillisecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uMillisecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        uMillisecond = uMillisecondsInLastSecond / HNS_PER_MILLISECOND;
    }

    return scast_q(uMillisecond, unsigned int);
}

unsigned int QDateTime::GetMicrosecond() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent microseconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMicrosecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uMicrosecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        u64_q uMicrosecondsInLastMillisecond = uMillisecondsInLastSecond % HNS_PER_MILLISECOND;
        uMicrosecond = uMicrosecondsInLastMillisecond / HNS_PER_MICROSECOND;
    }

    return scast_q(uMicrosecond, unsigned int);
}

unsigned int QDateTime::GetHundredOfNanosecond() const
{
    QE_ASSERT(!this->IsUndefined(), "Undefined dates cannot represent nanoseconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uNanosecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    const u64_q INPUT_WITHOUT_OFFSET = IS_NEGATIVE_DATE ?
                                                         QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                         HNS_IN_INSTANT - QDateTime::HALF_VALUE;

    // Calculates the groups of 4 years that have passed since year zero
    const u64_q GROUPS_OF_4_YEARS = INPUT_WITHOUT_OFFSET / QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q YEARS_PASSED_PROVISIONAL = GROUPS_OF_4_YEARS * 4ULL;

    // Calculates the years that have passed since the last multiple of 4
    const u64_q HNS_IN_REMAINING_YEARS = INPUT_WITHOUT_OFFSET % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
    const u64_q REMAINING_YEARS = HNS_IN_REMAINING_YEARS / QDateTime::HNS_PER_YEAR;

    u64_q uYear = YEARS_PASSED_PROVISIONAL + REMAINING_YEARS;

    // Read the technical documentation to know why it is 2 the value to compare to
    const bool IS_LEAP_YEAR = REMAINING_YEARS > 2ULL;

    u64_q uHnsInLastYear = 0;
    
    if(!IS_NEGATIVE_DATE)
    {
        if(REMAINING_YEARS == 4) // This occurs the last day of a leap year
        {
            uHnsInLastYear = (HNS_IN_REMAINING_YEARS + QDateTime::HNS_PER_LEAPYEAR) % QDateTime::HNS_PER_4_CONSECUTIVE_YEARS;
        }
        else
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;
            uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
        }
    }
    else
    {
        uHnsInLastYear = HNS_IN_REMAINING_YEARS % QDateTime::HNS_PER_YEAR;
    }

    if(uHnsInLastYear == 0)
    {
        uNanosecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // Adds 1 year to compensate the year subtracted previously due to the year zero gap
            ++uYear;

            if(IS_LEAP_YEAR)
            {
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            }
            else
            {
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
            }
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        u64_q uMicrosecondsInLastMillisecond = uMillisecondsInLastSecond % HNS_PER_MILLISECOND;
        uNanosecond = uMicrosecondsInLastMillisecond % HNS_PER_MICROSECOND;
    }

    return scast_q(uNanosecond, unsigned int);
}

QDateTime QDateTime::GetUtc() const
{
    return QDateTime(*this, null_q);
}

const QDateTime& QDateTime::GetMaxDateTime()
{
    static const QDateTime MAXIMUM_DATETIME(29228, 2, 8, 2, 48, 5, 477, 580, 7);
    return MAXIMUM_DATETIME;
}

const QDateTime& QDateTime::GetMinDateTime()
{
    static const QDateTime MINIMUM_DATETIME(-29228, 11, 23, 21, 11, 54, 522, 419, 3);
    return MINIMUM_DATETIME;
}

bool QDateTime::IsPositive() const
{
    bool bIsPositive = false;

    if(this->IsUndefined())
    {
        QE_ASSERT(false, "The date is undefined, it cannot be positive");
    }
    else if(m_pTimeZone == null_q)
    {
        bIsPositive = m_instant.GetHundredsOfNanoseconds() >= QDateTime::HALF_VALUE;
    }
    else
    {
        // Adds the time zone offset before checking
        QTimeSpan localTime = m_instant;
        localTime = this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);
        bIsPositive = localTime.GetHundredsOfNanoseconds() >= QDateTime::HALF_VALUE;
    }

    return bIsPositive;
}

bool QDateTime::IsNegative() const
{
    bool bIsNegative = false;

    if(this->IsUndefined())
    {
        QE_ASSERT(false, "The date is undefined, it cannot be negative");
    }
    else if(m_pTimeZone == null_q)
    {
        bIsNegative = m_instant.GetHundredsOfNanoseconds() < QDateTime::HALF_VALUE;
    }
    else
    {
        // Adds the time zone offset before checking
        QTimeSpan localTime = m_instant;
        localTime = this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);
        bIsNegative = localTime.GetHundredsOfNanoseconds() < QDateTime::HALF_VALUE;
    }

    return bIsNegative;
}

bool QDateTime::IsUndefined() const
{
    return m_instant == QDateTime::UNDEFINED_VALUE;
}

} //namespace Time
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis
