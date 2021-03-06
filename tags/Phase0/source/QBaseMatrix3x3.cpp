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

#include "QBaseMatrix3x3.h"

#include "SQFloat.h"
#include "SQVF32.h"

using Kinesis::QuimeraEngine::Tools::DataTypes::SQFloat;


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Math
{


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |       CONSTRUCTORS		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

QBaseMatrix3x3::QBaseMatrix3x3()
{
	ij[0][0] = ij[0][1] = ij[0][2] = SQFloat::_0;
	ij[1][0] = ij[1][1] = ij[1][2] = SQFloat::_0;
	ij[2][0] = ij[2][1] = ij[2][2] = SQFloat::_0;
}

QBaseMatrix3x3::QBaseMatrix3x3(const QBaseMatrix3x3 &matrix)
{
    memcpy(this->ij, matrix.ij, sizeof(float_q) * 9);
}

QBaseMatrix3x3::QBaseMatrix3x3(const float_q &fValueAll)
{
	ij[0][0] = ij[0][1] = ij[0][2] =
	ij[1][0] = ij[1][1] = ij[1][2] =
	ij[2][0] = ij[2][1] = ij[2][2] = fValueAll;
}

QBaseMatrix3x3::QBaseMatrix3x3(const float_q &f00, const float_q &f01, const float_q &f02,
                               const float_q &f10, const float_q &f11, const float_q &f12,
                               const float_q &f20, const float_q &f21, const float_q &f22)
{
	ij[0][0] = f00;
	ij[0][1] = f01;
	ij[0][2] = f02;
	ij[1][0] = f10;
	ij[1][1] = f11;
	ij[1][2] = f12;
	ij[2][0] = f20;
	ij[2][1] = f21;
	ij[2][2] = f22;
}

QBaseMatrix3x3::QBaseMatrix3x3(const float_q* arValues)
{
	QE_ASSERT(arValues != null_q)

	ij[0][0] = arValues[0];
	ij[0][1] = arValues[1];
	ij[0][2] = arValues[2];
	ij[1][0] = arValues[3];
	ij[1][1] = arValues[4];
	ij[1][2] = arValues[5];
	ij[2][0] = arValues[6];
	ij[2][1] = arValues[7];
	ij[2][2] = arValues[8];
}

QBaseMatrix3x3::QBaseMatrix3x3(const vf32_q &row0, const vf32_q &row1, const vf32_q &row2)
{
    using Kinesis::QuimeraEngine::Tools::DataTypes::SQVF32;

	float_q fAux;

	SQVF32::Unpack(row0, this->ij[0][0], this->ij[0][1], this->ij[0][2], fAux);
	SQVF32::Unpack(row1, this->ij[1][0], this->ij[1][1], this->ij[1][2], fAux);
	SQVF32::Unpack(row2, this->ij[2][0], this->ij[2][1], this->ij[2][2], fAux);
}

	
//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |		    METHODS			 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

bool QBaseMatrix3x3::operator==(const QBaseMatrix3x3 &matrix) const
{
    return SQFloat::AreEqual(this->ij[0][0], matrix.ij[0][0]) &&
           SQFloat::AreEqual(this->ij[0][1], matrix.ij[0][1]) &&
           SQFloat::AreEqual(this->ij[0][2], matrix.ij[0][2]) &&
           SQFloat::AreEqual(this->ij[1][0], matrix.ij[1][0]) &&
           SQFloat::AreEqual(this->ij[1][1], matrix.ij[1][1]) &&
           SQFloat::AreEqual(this->ij[1][2], matrix.ij[1][2]) &&
           SQFloat::AreEqual(this->ij[2][0], matrix.ij[2][0]) &&
           SQFloat::AreEqual(this->ij[2][1], matrix.ij[2][1]) &&
           SQFloat::AreEqual(this->ij[2][2], matrix.ij[2][2]);
}

bool QBaseMatrix3x3::operator!=(const QBaseMatrix3x3 &matrix) const
{
    return  !(*this == matrix);
}

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis
