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

#include "SQAnyTypeToStringConverter.h"

#include "SQFloat.h"
#include "SQVF32.h"
#include "SQBoolean.h"

using Kinesis::QuimeraEngine::Core::QObject;


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Common
{
namespace DataTypes
{


//##################=======================================================##################
//##################             ____________________________              ##################
//##################            |                            |             ##################
//##################            |           METHODS          |             ##################
//##################           /|                            |\            ##################
//##################             \/\/\/\/\/\/\/\/\/\/\/\/\/\/              ##################
//##################                                                       ##################
//##################=======================================================##################

bool SQAnyTypeToStringConverter::IsObject(const void* pObject)
{
    return false;
}

bool SQAnyTypeToStringConverter::IsObject(const Kinesis::QuimeraEngine::Core::QObject* pObject)
{
    return true;
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const bool &object)
{
    return SQBoolean::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const u8_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const u16_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const u32_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const u64_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const i8_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const i16_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const i32_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const i64_q &object)
{
    return SQInteger::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const f32_q &object)
{
    // [TODO] Thund: Change this when an method of SQFloat to convert 32 bits floats exists
    std::ostringstream output;
    output << std::setprecision(9) << object;

    return output.str().c_str();
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const f64_q &object)
{
    // [TODO] Thund: Change this when an method of SQFloat to convert 64 bits floats exists
    std::ostringstream output;
    output << std::setprecision(17) << object;

    return output.str().c_str();
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const vf32_q &object)
{
    return SQVF32::ToString(object);
}

template<>
string_q SQAnyTypeToStringConverter::Convert(const string_q &object)
{
    return object;
}



} //namespace DataTypes
} //namespace Common
} //namespace QuimeraEngine
} //namespace Kinesis
