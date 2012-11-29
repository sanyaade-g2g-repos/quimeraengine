// [TERMS&CONDITIONS]

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
using namespace boost::unit_test;

#include "../../../testsystem/TestingExternalDefinitions.h"

#include "STATStringHelper.h"

using Kinesis::TestAutomationTool::Backend::STATStringHelper;

QTEST_SUITE_BEGIN( STATStringHelper_TestSuite )

/// <summary>
/// Checks that an empty list is returned when the input string is empty.
/// </summary>
QTEST_CASE ( Split_ReturnsEmptyListWhenInputIsEmpty_Test )
{
    // Preparation
    const wxString EMPTY_INPUT = wxT("");
    const wxString SEPARATOR = wxT("-SEPARATOR-");
    const bool RESULT_IS_EMPTY = true;

	// Execution
    std::list<wxString> result = STATStringHelper::Split(EMPTY_INPUT, SEPARATOR);
    
    // Verification
    BOOST_CHECK_EQUAL(result.empty(), RESULT_IS_EMPTY);
}

/// <summary>
/// Checks that a list with just one entry is returned, which contains the entire input string, when the separator token is empty.
/// </summary>
QTEST_CASE ( Split_ReturnsJustOneEntryWithTheEntireInputWhenSeparatorIsEmpty_Test )
{
    // Preparation
    const wxString INPUT = wxT("Con-SEPARATOR-tent-SEPARATOR-");
    const wxString EMPTY_SEPARATOR = wxT("");
    std::list<wxString> EXPECTED_RESULT;
    EXPECTED_RESULT.push_back(INPUT);

	// Execution
    std::list<wxString> result = STATStringHelper::Split(INPUT, EMPTY_SEPARATOR);
    
    // Verification
    BOOST_CHECK_EQUAL(result.size(), EXPECTED_RESULT.size());
    BOOST_CHECK_EQUAL(*result.begin(), *EXPECTED_RESULT.begin());
}

/// <summary>
/// Checks that correct entries are returned when input string doesn't end with a separator.
/// </summary>
QTEST_CASE ( Split_ReturnsCorrectEntriesWhenInputDoesntEndWithASeparator_Test )
{
    // Preparation
    const wxString ENTRY1 = wxString("Con");
    const wxString ENTRY2 = wxString("tent");
    const wxString SEPARATOR = wxT("-SEPARATOR-");
    const wxString INPUT = ENTRY1 + SEPARATOR + ENTRY2;
    std::list<wxString> EXPECTED_RESULT;
    EXPECTED_RESULT.push_back(ENTRY1);
    EXPECTED_RESULT.push_back(ENTRY2);

	// Execution
    std::list<wxString> result = STATStringHelper::Split(INPUT, SEPARATOR);
    
    // Verification
    BOOST_CHECK_EQUAL(result.size(), EXPECTED_RESULT.size());

    for(std::list<wxString>::iterator resultEntry = result.begin(), expectedEntry = EXPECTED_RESULT.begin(); resultEntry != result.end(); ++resultEntry, ++expectedEntry)
    {
        BOOST_CHECK_EQUAL(*resultEntry, *expectedEntry);
    }
}

/// <summary>
/// Checks that correct entries are returned when input string ends with a separator.
/// </summary>
QTEST_CASE ( Split_ReturnsCorrectEntriesWhenInputEndsWithASeparator_Test )
{
    // Preparation
    const wxString ENTRY1 = wxString("Con");
    const wxString ENTRY2 = wxString("tent");
    const wxString SEPARATOR = wxT("-SEPARATOR-");
    const wxString INPUT = ENTRY1 + SEPARATOR + ENTRY2 + SEPARATOR;
    std::list<wxString> EXPECTED_RESULT;
    EXPECTED_RESULT.push_back(ENTRY1);
    EXPECTED_RESULT.push_back(ENTRY2);

	// Execution
    std::list<wxString> result = STATStringHelper::Split(INPUT, SEPARATOR);
    
    // Verification
    BOOST_CHECK_EQUAL(result.size(), EXPECTED_RESULT.size());

    for(std::list<wxString>::iterator resultEntry = result.begin(), expectedEntry = EXPECTED_RESULT.begin(); resultEntry != result.end(); ++resultEntry, ++expectedEntry)
    {
        BOOST_CHECK_EQUAL(*resultEntry, *expectedEntry);
    }
}

/// <summary>
/// Checks that just one entry is returned, which contains the entire input string, when the separator token is not found in it.
/// </summary>
QTEST_CASE ( Split_ReturnsJustOneEntryWithTheEntireInputWhenSeparatorIsNotFound_Test )
{
    // Preparation
    const wxString ENTRY1 = wxString("Con");
    const wxString ENTRY2 = wxString("tent");
    const wxString SEPARATOR = wxT("-SEPARATOR-");
    const wxString INPUT = ENTRY1 + ENTRY2;
    std::list<wxString> EXPECTED_RESULT;
    EXPECTED_RESULT.push_back(INPUT);

	// Execution
    std::list<wxString> result = STATStringHelper::Split(INPUT, SEPARATOR);
    
    // Verification
    BOOST_CHECK_EQUAL(result.size(), EXPECTED_RESULT.size());
    BOOST_CHECK_EQUAL(*result.begin(), *EXPECTED_RESULT.begin());
}

/// <summary>
/// Checks that just one empty entry is returned when the input strings matches the separator token.
/// </summary>
QTEST_CASE ( Split_ReturnsJustOneEmptyEntryWhenInputEqualsSeparator_Test )
{
    // Preparation
    const wxString SEPARATOR = wxT("-SEPARATOR-");
    const wxString INPUT = SEPARATOR;
    std::list<wxString> EXPECTED_RESULT;
    EXPECTED_RESULT.push_back(wxT(""));

	// Execution
    std::list<wxString> result = STATStringHelper::Split(INPUT, SEPARATOR);
    
    // Verification
    BOOST_CHECK_EQUAL(result.size(), EXPECTED_RESULT.size());
    BOOST_CHECK_EQUAL(*result.begin(), *EXPECTED_RESULT.begin());
}

// End - Test Suite: STATStringHelper
QTEST_SUITE_END()
