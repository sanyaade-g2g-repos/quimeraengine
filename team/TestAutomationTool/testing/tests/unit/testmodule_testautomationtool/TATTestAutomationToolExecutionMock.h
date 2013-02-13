//-------------------------------------------------------------------------------//
//                   TEST AUTOMATION TOOL : LICENSE                              //
//-------------------------------------------------------------------------------//
// This file is part of Test Automation Tool.                                    //
// Test Automation Tool is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation, either version 3 of the License, or             //
// (at your option) any later version.                                           //
//                                                                               //
// Test Automation Tool is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License for more details.                                  //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with Test Automation Tool. If not, see <http://www.gnu.org/licenses/>.  //
//                                                                               //
// This license doesn't force you to put any kind of banner or logo telling      //
// that you are using Test Automation Tool in your project but we would          //
// appreciate if you do so or, at least, if you let us know about that.          //
//                                                                               //
// Enjoy!                                                                        //
//                                                                               //
// Kinesis Team                                                                  //
//-------------------------------------------------------------------------------//

#ifndef __TATTESTAUTOMATIONTOOLEXECUTIONMOCK__
#define __TATTESTAUTOMATIONTOOLEXECUTIONMOCK__

#include "TestExecution/TATTestAutomationToolExecution.h"

namespace Kinesis
{
namespace TestAutomationTool
{
namespace Backend
{
namespace Test
{

/// <summary>
/// A mock derived from TATTestAutomationToolExecution for testing purposes.
/// </summary>
class TATTestAutomationToolExecutionMock : public TATTestAutomationToolExecution
{
public:

    TATTestAutomationToolExecutionMock(ITATLogger* logger)
    {
        m_pLogger = logger;
    }

    TATTestAutomationToolExecutionMock(ITATTestResultLoader* pTestResultLoader)
    {
        m_pTestResultLoader = pTestResultLoader;
    }
};


} //namespace Test
} //namespace Backend
} //namespace TestAutomationTool
} //namespace Kinesis

#endif // __TATTESTAUTOMATIONTOOLEXECUTIONMOCK__
