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

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
using namespace boost::unit_test;

#include "../../testsystem/TestingExternalDefinitions.h"

#include "QKeyValuePair.h"
#include "StringsDefinitions.h"

using Kinesis::QuimeraEngine::Tools::Containers::QKeyValuePair;
using Kinesis::QuimeraEngine::Common::DataTypes::string_q;


QTEST_SUITE_BEGIN( QKeyValuePair_TestSuite )

/// <summary>
/// Checks that all parameters are stored correctly
/// </summary>
QTEST_CASE ( Constructor_ParametersAreCorrectlyStored_Test )
{
    // [Preparation]
    const string_q EXPECTED_KEY("KEY");
    const int EXPECTED_VALUE = 5;

    // [Execution]
    QKeyValuePair<string_q, int> kvpair(EXPECTED_KEY, EXPECTED_VALUE);

    // [Verification]
    BOOST_CHECK(kvpair.GetKey() == EXPECTED_KEY);
    BOOST_CHECK_EQUAL(kvpair.GetValue(), EXPECTED_VALUE);
}

// End - Test Suite: QKeyValuePair
QTEST_SUITE_END()
