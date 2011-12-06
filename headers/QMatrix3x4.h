// [TERMS&CONDITIONS]

#ifndef __QMATRIX3X4__
#define __QMATRIX3X4__

#include "QBaseMatrix3x3.h"
#include "QBaseMatrix4x4.h"
#include "QBaseMatrix3x4.h"
#include "QBaseMatrix4x3.h"

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
/// This class implements the functionality of a matrix with 3 rows and 4 columns.
///
/// A matrix is a rectangular arrangement of numbers. The horizontal and vertical lines in a matrix
/// are called rows and columns, respectively. The numbers in the matrix are called its entries or its elements.
/// To specify the size of a matrix, a matrix with m rows and n columns is called an m-by-n matrix or m x n matrix,
/// while m and n are called its dimensions. Every element is referenced by its position in the matrix.
/// Due to we use a row by column convention, we will always write first the row of the element and then its
/// column: the element in the i row and the j column is denoted \f$ A_{ij} \f$. In this case, we will work with 4x3 matrices,
/// therefore our matrix will be:
///
/// \f$ A = \begin{bmatrix} a_{00} & a_{01} & a_{02} & a_{03}\\ a_{10} & a_{11} & a_{12} & a_{13}\\ a_{20} & a_{21} & a_{22} & a_{23}\end{bmatrix}\f$
///
/// </summary>
class QDllExport QMatrix3x4 : public QBaseMatrix3x4
{
	// FRIENDS
	// ---------------
public:
	/// <summary>
	/// Multiply by scalar operator. All matrix components are multiplied by the scalar.
	/// </summary>
	/// <param name="fScalar">[IN] The scalar term of product.</param>
	/// <param name="m">[IN] The matrix term of product.</param>
	/// <returns>
	/// The resultant matrix.
	/// </returns>
	friend QMatrix3x4 operator*(const float_q &fScalar, const QBaseMatrix3x4 &m);

	// CONSTANTS
	// ---------------
public:

	/// <summary>
	/// Stores a matrix with all components setted to 0.
	/// </summary>
	static const QMatrix3x4 ZeroMatrix;
    /// <summary>
    /// Stores an identity matrix.
    /// The identity matrix is a matrix whose elements are zero except the main diagonal that is composed by ones:
    ///
    /// \f$ I = \begin{bmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 1 & 0 \end{bmatrix}\f$
    ///
    /// </summary>
    static const QMatrix3x4 Identity;

 	// CONSTRUCTORS
	// ---------------
public:

	/// <summary>
	/// Default constructor.
	/// </summary>
	inline QMatrix3x4() { }

	/// <summary>
	/// Constructor from a 3x4 matrix.
	/// </summary>
	/// <param name="m">[IN] The 3x4 matrix in which we want the resident matrix to be based.</param>
	inline explicit QMatrix3x4(const QBaseMatrix3x4 &m) : QBaseMatrix3x4(m) { }

	/// <summary>
	/// Constructor from a floating point value which with fill all matrix's elements.
	/// </summary>
	/// <param name="fValue">[IN] The floating point value used to fill the matrix.</param>
	inline explicit QMatrix3x4(const float_q &fValue) : QBaseMatrix3x4(fValue) { }

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
	inline QMatrix3x4(	const float_q &f00, const float_q &f01, const float_q &f02, const float_q &f03,
						const float_q &f10, const float_q &f11, const float_q &f12, const float_q &f13,
						const float_q &f20, const float_q &f21, const float_q &f22, const float_q &f23) :

						QBaseMatrix3x4(f00, f01, f02, f03,
							           f10, f11, f12, f13,
									   f20, f21, f22, f23) {}

	/// <summary>
	/// Constructor that receives a pointer to 12 floating point values.
	/// </summary>
	/// <remarks>
	/// Keeps the convention rows x columns, so each chunck of 3 consecutive elements
	/// corresponds to a row, where each element in the chunck is the column in the row.
	/// </remarks>
	/// <param name="pfMatrix">[IN] Pointer to a 12 length array of floating point values.</param>
	inline explicit QMatrix3x4(const float_q *pfMatrix) : QBaseMatrix3x4(pfMatrix) { }

	/// <summary>
	/// Constructor from three 4x32 floating point packed values. Each param contains a row of the matrix.
	/// </summary>
	/// <param name="row0">[IN] 4x32 values for row 0, columns 0 to 3 unpacked in this order.</param>
	/// <param name="row1">[IN] 4x32 values for row 1, columns 0 to 3 unpacked in this order.</param>
	/// <param name="row2">[IN] 4x32 values for row 2, columns 0 to 3 unpacked in this order.</param>
	inline QMatrix3x4(const vf32_q &row0, const vf32_q &row1, const vf32_q &row2) :
		QBaseMatrix3x4(row0, row1, row2) { }

	// METHODS
	// ---------------
public:

	/// <summary>
	/// Multiply by scalar operator. All matrix components are multiplied by the scalar.
	/// </summary>
	/// <param name="fScalar">[IN] The scalar to multiply by.</param>
	/// <returns>
	/// The resultant matrix.
	/// </returns>
	QMatrix3x4 operator*(const float_q &fScalar) const;

	/// <summary>
	/// Multiply the resident matrix by a 4x3 matrix.
	/// </summary>
	/// <param name="m">[IN] A 4x3 matrix to be multiplied by.</param>
	/// <returns>
	/// The resultant 3x3 matrix.
	/// </returns>
	QBaseMatrix3x3 operator*(const QBaseMatrix4x3& m) const;

	/// <summary>
	/// Multiply the resident matrix by a 4x4 matrix.
	/// </summary>
	/// <param name="m">[IN] A 4x4 matrix to be multiplied by.</param>
	/// <returns>
	/// The resultant 3x4 matrix.
	/// </returns>
	QBaseMatrix3x4 operator*(const QBaseMatrix4x4& m) const;

	/// <summary>
	/// Divides resident matrix by a floating point value.
	/// </summary>
	/// <param name="fScalar">[IN] Floating point value to be divided by.</param>
	/// <returns>
	/// The resultant matrix.
	/// </returns>
	QMatrix3x4 operator/(const float_q &fScalar) const;

	/// <summary>
	/// Adds a 3x4 matrix to the resident matrix.
	/// </summary>
	/// <param name="m">[IN] The matrix to be added to.</param>
	/// <returns>
	/// The resultant matrix.
	/// </returns>
	QMatrix3x4 operator+(const QBaseMatrix3x4 &m) const;

	/// <summary>
	/// Subtracts a QMatrix3x4 to the resident matrix.
	/// </summary>
	/// <param name="m">[IN] The matrix to be subtracted to.</param>
	/// <returns>
	/// The resultant matrix.
	/// </returns>
	QMatrix3x4 operator-(const QBaseMatrix3x4 &m) const;

	/// <summary>
	/// Division and assign operator. Resident matrix stores the result of the division.
	/// </summary>
	/// <param name="fScalar">[IN] The floating point value by which to divide.</param>
	/// <returns>
	/// The modified matrix.
	/// </returns>
	inline QMatrix3x4& operator/=(const float_q &fScalar)
	{
		QE_ASSERT(fScalar != SQFloat::_0);

		this->ij[0][0] /= fScalar;
		this->ij[0][1] /= fScalar;
		this->ij[0][2] /= fScalar;
		this->ij[0][3] /= fScalar;
		this->ij[1][0] /= fScalar;
		this->ij[1][1] /= fScalar;
		this->ij[1][2] /= fScalar;
		this->ij[1][3] /= fScalar;
		this->ij[2][0] /= fScalar;
		this->ij[2][1] /= fScalar;
		this->ij[2][2] /= fScalar;
		this->ij[2][3] /= fScalar;

		return *this;
	}

	/// <summary>
	/// Addition and assign operator. Resident matrix stores the result of the addition.
	/// </summary>
	/// <param name="m">[IN] The matrix to be added to.</param>
	/// <returns>
	/// The modified matrix.
	/// </returns>
	inline QMatrix3x4& operator+=(const QBaseMatrix3x4 &m)
	{
		this->ij[0][0] += m.ij[0][0];
		this->ij[0][1] += m.ij[0][1];
		this->ij[0][2] += m.ij[0][2];
		this->ij[0][3] += m.ij[0][3];
		this->ij[1][0] += m.ij[1][0];
		this->ij[1][1] += m.ij[1][1];
		this->ij[1][2] += m.ij[1][2];
		this->ij[1][3] += m.ij[1][3];
		this->ij[2][0] += m.ij[2][0];
		this->ij[2][1] += m.ij[2][1];
		this->ij[2][2] += m.ij[2][2];
		this->ij[2][3] += m.ij[2][3];

		return *this;
	}

	/// <summary>
	/// Subtraction and assign operator. Resident matrix stores the result of the subtraction.
	/// </summary>
	/// <param name="m">[IN] The matrix to be subtracted to.</param>
	/// <returns>
	/// The modified matrix.
	/// </returns>
	inline QMatrix3x4& operator-=(const QBaseMatrix3x4 &m)
	{
		this->ij[0][0] -= m.ij[0][0];
		this->ij[0][1] -= m.ij[0][1];
		this->ij[0][2] -= m.ij[0][2];
		this->ij[0][3] -= m.ij[0][3];
		this->ij[1][0] -= m.ij[1][0];
		this->ij[1][1] -= m.ij[1][1];
		this->ij[1][2] -= m.ij[1][2];
		this->ij[1][3] -= m.ij[1][3];
		this->ij[2][0] -= m.ij[2][0];
		this->ij[2][1] -= m.ij[2][1];
		this->ij[2][2] -= m.ij[2][2];
		this->ij[2][3] -= m.ij[2][3];

		return *this;
	}

	/// <summary>
    /// Product and assign operator. Current matrix stores the result of the multiplication.
    /// </summary>
    /// <param name="m">[IN] A 4x4 matrix to be multiplied by.</param>
    /// <returns>
    /// The modified matrix.
    /// </returns>
    inline QMatrix3x4& operator*=(const QBaseMatrix4x4 &m)
	{
		QMatrix3x4 aux;

		aux.ij[0][0] = this->ij[0][0] * m.ij[0][0] + this->ij[0][1] * m.ij[1][0] + this->ij[0][2] * m.ij[2][0] + this->ij[0][3] * m.ij[3][0];
		aux.ij[0][1] = this->ij[0][0] * m.ij[0][1] + this->ij[0][1] * m.ij[1][1] + this->ij[0][2] * m.ij[2][1] + this->ij[0][3] * m.ij[3][1];
		aux.ij[0][2] = this->ij[0][0] * m.ij[0][2] + this->ij[0][1] * m.ij[1][2] + this->ij[0][2] * m.ij[2][2] + this->ij[0][3] * m.ij[3][2];
		aux.ij[0][3] = this->ij[0][0] * m.ij[0][3] + this->ij[0][1] * m.ij[1][3] + this->ij[0][2] * m.ij[2][3] + this->ij[0][3] * m.ij[3][3];

		aux.ij[1][0] = this->ij[1][0] * m.ij[0][0] + this->ij[1][1] * m.ij[1][0] + this->ij[1][2] * m.ij[2][0] + this->ij[1][3] * m.ij[3][0];
		aux.ij[1][1] = this->ij[1][0] * m.ij[0][1] + this->ij[1][1] * m.ij[1][1] + this->ij[1][2] * m.ij[2][1] + this->ij[1][3] * m.ij[3][1];
		aux.ij[1][2] = this->ij[1][0] * m.ij[0][2] + this->ij[1][1] * m.ij[1][2] + this->ij[1][2] * m.ij[2][2] + this->ij[1][3] * m.ij[3][2];
		aux.ij[1][3] = this->ij[1][0] * m.ij[0][3] + this->ij[1][1] * m.ij[1][3] + this->ij[1][2] * m.ij[2][3] + this->ij[1][3] * m.ij[3][3];

		aux.ij[2][0] = this->ij[2][0] * m.ij[0][0] + this->ij[2][1] * m.ij[1][0] + this->ij[2][2] * m.ij[2][0] + this->ij[2][3] * m.ij[3][0];
		aux.ij[2][1] = this->ij[2][0] * m.ij[0][1] + this->ij[2][1] * m.ij[1][1] + this->ij[2][2] * m.ij[2][1] + this->ij[2][3] * m.ij[3][1];
		aux.ij[2][2] = this->ij[2][0] * m.ij[0][2] + this->ij[2][1] * m.ij[1][2] + this->ij[2][2] * m.ij[2][2] + this->ij[2][3] * m.ij[3][2];
		aux.ij[2][3] = this->ij[2][0] * m.ij[0][3] + this->ij[2][1] * m.ij[1][3] + this->ij[2][2] * m.ij[2][3] + this->ij[2][3] * m.ij[3][3];

		*this = aux;

		return *this;
	}

	/// <summary>
	/// Resets all matrix elements to 0.
	/// </summary>
	inline void ResetToZero()
	{
		this->ij[0][0] = this->ij[0][1] = this->ij[0][2] = this->ij[0][3] =
		this->ij[1][0] = this->ij[1][1] = this->ij[1][2] = this->ij[1][3] =
		this->ij[2][0] = this->ij[2][1] = this->ij[2][2] = this->ij[2][3] = SQFloat::_0;
	}

	/// <summary>
	/// Transpose: the transpose of a matrix m x n is a matrix n x m where each row becomes a column
	/// and each column becomes a row. Every element Aij becomes Aji. It's noted A^T
 	/// </summary>
	/// <remarks>
	/// If the matrix is a rotation matrix, then the transpose is guaranteed to be the inverse of the matrix.
	/// </remarks>
	/// <param name="m">[OUT] Stores the resultant trasposed matrix. Output matrix is 4x3</param>
	inline void Transpose(QBaseMatrix4x3 &m) const
	{
		m.ij[0][0] = this->ij[0][0];
		m.ij[1][0] = this->ij[0][1];
		m.ij[2][0] = this->ij[0][2];
		m.ij[3][0] = this->ij[0][3];

		m.ij[0][1] = this->ij[1][0];
		m.ij[1][1] = this->ij[1][1];
		m.ij[2][1] = this->ij[1][2];
		m.ij[3][1] = this->ij[1][3];

		m.ij[0][2] = this->ij[2][0];
		m.ij[1][2] = this->ij[2][1];
		m.ij[2][2] = this->ij[2][2];
		m.ij[3][2] = this->ij[2][3];

	}

	/// <summary>
	/// Checks if all elements of the matrix are 0 or under tolerance (absolute value).
	/// </summary>
	/// <returns>
	/// True if all elements are 0, false otherwise.
	/// </returns>
	inline bool IsZero() const
	{
		return	SQFloat::IsZero(this->ij[0][0]) &&
				SQFloat::IsZero(this->ij[0][1]) &&
				SQFloat::IsZero(this->ij[0][2]) &&
				SQFloat::IsZero(this->ij[0][3]) &&
				SQFloat::IsZero(this->ij[1][0]) &&
				SQFloat::IsZero(this->ij[1][1]) &&
				SQFloat::IsZero(this->ij[1][2]) &&
				SQFloat::IsZero(this->ij[1][3]) &&
				SQFloat::IsZero(this->ij[2][0]) &&
				SQFloat::IsZero(this->ij[2][1]) &&
				SQFloat::IsZero(this->ij[2][2]) &&
				SQFloat::IsZero(this->ij[2][3]);
	}

    /// <summary>
    /// Assign operator. Assigns the provided matrix to the resident matrix.
    /// </summary>
    /// <param name="m">[IN] The matrix to be assigned.</param>
    /// <returns>
    /// A reference to the modified matrix.
    /// </returns>
    inline QMatrix3x4& operator=(const QBaseMatrix3x4 &m)
    {
        QBaseMatrix3x4::operator=(m);
		return *this;
    }

	/// <summary>
    /// Resets the matrix to a identity matrix. The element \f$ A_{ij} \f$ is set to 0 if \f$ i\neq j \f$,
    /// and it's set to 1 if \f$ i=j\f$.
    /// </summary>
    inline void ResetToIdentity()
    {
        this->ij[0][0] = this->ij[1][1] = this->ij[2][2] = SQFloat::_1;

        this->ij[0][1] = this->ij[0][2] = this->ij[0][3] =
        this->ij[1][0] = this->ij[1][2] = this->ij[1][3] =
        this->ij[2][0] = this->ij[2][1] = this->ij[2][3] = SQFloat::_0;
    }

	/// <summary>
	/// Converts matrix into a string with the following format:
	/// "M3x4( 11, 12, 13, 14 )( 21, 22, 23, 24 )( 31, 32, 33, 34 )"
	/// </summary>
	/// <returns>The string with the format specified.</returns>
    inline string_q ToString() const
    {
        return QE_L("M3x4(") + SQFloat::ToString(this->ij[0][0]) + QE_L(",") +
                               SQFloat::ToString(this->ij[0][1]) + QE_L(",") +
							   SQFloat::ToString(this->ij[0][2]) + QE_L(",") +
                               SQFloat::ToString(this->ij[0][3]) + QE_L(")(") +
                               SQFloat::ToString(this->ij[1][0]) + QE_L(",") +
                               SQFloat::ToString(this->ij[1][1]) + QE_L(",") +
							   SQFloat::ToString(this->ij[1][2]) + QE_L(",") +
                               SQFloat::ToString(this->ij[1][3]) + QE_L(")(") +
                               SQFloat::ToString(this->ij[2][0]) + QE_L(",") +
                               SQFloat::ToString(this->ij[2][1]) + QE_L(",") +
							   SQFloat::ToString(this->ij[2][2]) + QE_L(",") +
                               SQFloat::ToString(this->ij[2][3]) + QE_L(")");
    }
};

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QMATRIX3x4__
