// [TERMS&CONDITIONS]

#ifndef __QBASEMATRIX3X4__
#define __QBASEMATRIX3X4__

#include "QFloat.h"
#include "QVF32.h"

using namespace Kinesis::QuimeraEngine::Tools::DataTypes;

namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Math
{

/// <summary>
/// Base class to represent a matrix of floating point values with 3 rows and 4 columns.
/// </summary>
class QDllExport QBaseMatrix3x4
{
	// CONSTRUCTORS
	// ---------------
public:

	/// <summary>
	/// Default constructor. Initializes each element in the matrix to 0.
	/// </summary>
	inline QBaseMatrix3x4()
	{
		ij[0][0] = ij[0][1] = ij[0][2] = ij[0][3] = 
		ij[1][0] = ij[1][1] = ij[1][2] = ij[1][3] = 
		ij[2][0] = ij[2][1] = ij[2][2] = ij[2][3] = QFloat::_0;		
	}

	/// <summary>
	/// Constructor that receives a floating point value for all elements of the matrix.
	/// </summary>
	/// <param name="fValue">[IN] Floating point value with which fills the matrix.</param>
	inline explicit QBaseMatrix3x4(const float_q &fValue)
	{
		ij[0][0] = ij[0][1] = ij[0][2] = ij[0][3] =
		ij[1][0] = ij[1][1] = ij[1][2] = ij[1][3] =
		ij[2][0] = ij[2][1] = ij[2][2] = ij[2][3] = fValue;
	}

	/// <summary>
	/// Constructor from a floating point value for each element of the matrix.
	/// </summary>
	/// <param name="f00">[IN] Floating point value for element of row 0, column 0.</param>
	/// <param name="f01">[IN] Floating point value for element of row 0, column 1.</param>
	/// <param name="f02">[IN] Floating point value for element of row 0, column 2.</param>
	/// <param name="f03">[IN] Floating point value for element of row 0, column 3.</param>
	/// <param name="f10">[IN] Floating point value for element of row 1, column 0.</param>
	/// <param name="f11">[IN] Floating point value for element of row 1, column 1.</param>
	/// <param name="f12">[IN] Floating point value for element of row 1, column 2.</param>
	/// <param name="f13">[IN] Floating point value for element of row 1, column 3.</param>
	/// <param name="f20">[IN] Floating point value for element of row 2, column 0.</param>
	/// <param name="f21">[IN] Floating point value for element of row 2, column 1.</param>
	/// <param name="f22">[IN] Floating point value for element of row 2, column 2.</param>
	/// <param name="f23">[IN] Floating point value for element of row 2, column 3.</param>
	inline explicit QBaseMatrix3x4(	const float_q &f00, const float_q &f01, const float_q &f02, const float_q &f03,  
									const float_q &f10, const float_q &f11, const float_q &f12, const float_q &f13,  
									const float_q &f20, const float_q &f21, const float_q &f22, const float_q &f23)									
	{
		ij[0][0] = f00;
		ij[0][1] = f01; 
		ij[0][2] = f02;
		ij[0][3] = f03; 
		ij[1][0] = f10;
		ij[1][1] = f11;
		ij[1][2] = f12;
		ij[1][3] = f13; 
		ij[2][0] = f20;
		ij[2][1] = f21;
		ij[2][2] = f22;
		ij[2][3] = f23; 
	}	

	/// <summary>
	/// Constructor that receives a pointer to 12 floating point values.
	/// </summary>
	/// <remarks>
	/// Keeps the convention rows x columns, so each chunck of 4 consecutive elements 
	/// corresponds to a row, where each element in the chunck is the column in the row.
	/// </remarks>
	/// <param name="pfMatrix">[IN] Pointer to a 12 length array of floating point values.</param>
	inline QBaseMatrix3x4(const float_q *pfMatrix)
	{
		QE_ASSERT(pfMatrix != null_q);

		ij[0][0] = pfMatrix[0];
		ij[0][1] = pfMatrix[1];
		ij[0][2] = pfMatrix[2];
		ij[0][3] = pfMatrix[3];
		ij[1][0] = pfMatrix[4];
		ij[1][1] = pfMatrix[5];
		ij[1][2] = pfMatrix[6];
		ij[1][3] = pfMatrix[7];
		ij[2][0] = pfMatrix[8];
		ij[2][1] = pfMatrix[9];
		ij[2][2] = pfMatrix[10];
		ij[2][3] = pfMatrix[11];
	}

	/// <summary>
	/// Constructor from four 4x32 floating point packed values. Each param contains a row of the matrix.
	/// </summary>
	/// <param name="row0">[IN] 4x32 values for row 0, columns 0 to 3, parsed in this order.</param>
	/// <param name="row1">[IN] 4x32 values for row 1, columns 0 to 3, parsed in this order.</param>
	/// <param name="row2">[IN] 4x32 values for row 2, columns 0 to 3, parsed in this order.</param>
	inline QBaseMatrix3x4(const vf32_q &row0, const vf32_q &row1, const vf32_q &row2)
	{

		QVF32::Unpack(row0, this->ij[0][0], this->ij[0][1], this->ij[0][2], this->ij[0][3]);
		QVF32::Unpack(row1, this->ij[1][0], this->ij[1][1], this->ij[1][2], this->ij[1][3]);
		QVF32::Unpack(row2, this->ij[2][0], this->ij[2][1], this->ij[2][2], this->ij[2][3]);
	}

	// ATTRIBUTES
	// ---------------
public:

	/// <summary>
	/// Array that holds the matrix.
	/// </summary>
	float_q ij[3][4];
};

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QBASEMATRIX3X4__