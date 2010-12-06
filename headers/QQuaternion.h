// [TERMS&CONDITIONS]

#ifndef __QQUATERNION__
#define __QQUATERNION__

#include <string>

#include "QBaseQuaternion.h"

namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Math
{

/// <summary>
/// Quaternions extend the concept of rotation in three dimensions to
/// rotation in four dimensions. This avoids the problem of "gimbal-lock"
/// and allows for the implementation of smooth and continuous rotation.
///
/// In effect, they may be considered to add a additional rotation angle
/// to spherical coordinates ie. Longitude, Latitude and Rotation angles
///
/// A Quaternion is defined using four floating point values |x y z w|.
///
/// These are calculated from the combination of the three coordinates
/// of the rotation axis and the rotation angle.
///
/// Text extracted from: Flipcode.
/// </summary>
class QDllExport QQuaternion : public QBaseQuaternion
{
    // FRIENDS
    // ---------------

    // [REVIEW] Thund: Does Doxygen catch this documentation for global functions?
    /// <summary>
    /// Multiply by scalar operator. All quaternion's components are multiplied by the scalar.
    /// </summary>
    /// <param name="fScalar">[IN] The scalar factor.</param>
    /// <param name="qQuat">[IN] The quaternion factor.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    friend QQuaternion operator*(const float_q &fScalar, const QQuaternion &qQuat);

public:

    // CONSTANTS
    // ---------------

    /// <summary>
    /// Represents the identity quaternion.
    /// </summary>
    static const QQuaternion Identity;


	// CONSTRUCTORS
	// ---------------
public:

	/// <summary>
	/// Default constructor.
	/// </summary>
    inline QQuaternion() : QBaseQuaternion() {};

    /// <summary>
    /// Constructor that receives a base quaternion.
    /// </summary>
    /// <param name="qQuat">[IN] A base quaternion.</param>
    inline explicit QQuaternion(const QBaseQuaternion &qQuat) : QBaseQuaternion(qQuat) {};

    /// <summary>
    /// Constructor that receives 3 angles, one for each Euler angle, to represent a spacial rotation as a quaternion.
    /// Quimera Engine follows the rotation order convention: Z, then X, then Y, aka Yaw-Pitch-Roll. This is a slow operation.
    /// </summary>
    /// <param name="fAngleX">[IN] Rotation angle about X global axis.</param>
    /// <param name="fAngleY">[IN] Rotation angle about Y global axis.</param>
    /// <param name="fAngleZ">[IN] Rotation angle about Z global axis.</param>
    QQuaternion(const float_q &fAngleX, const float_q &fAngleY, const float_q &fAngleZ);

    /// <summary>
    /// Constructor that receives 4 values, one per quaternion's component.
    /// </summary>
    /// <param name="fValueX">[IN] X component value.</param>
    /// <param name="fValueY">[IN] Y component value.</param>
    /// <param name="fValueZ">[IN] Z component value.</param>
    /// <param name="fValueW">[IN] W component value.</param>
    inline QQuaternion(const float_q &fValueX, const float_q &fValueY, const float_q &fValueZ, const float_q &fValueW)
         : QBaseQuaternion(fValueX, fValueY, fValueZ, fValueW) {};

    /// <summary>
    /// Constructor that receives a pointer to a sequence of 4 contiguous values, one per quaternion's component.
    /// </summary>
    /// <param name="pValues">[IN] Sequence of 4 contiguous values.</param>
    inline explicit QQuaternion(const float_q* pValues) : QBaseQuaternion(pValues) {};

    /// <summary>
    /// Constructor that receives four 32 bits floating point type, one per quaternion's component, packaged 
    /// into a 128 bits structure. The values order is: X, Y, Z and W.
    /// </summary>
    /// <param name="packedValues">[IN] A four 32 bits floating point types pack.</param>
    inline explicit QQuaternion(const vf32_q &packagedValues) : QBaseQuaternion(packagedValues) {};


	// METHODS
	// ---------------
public:

    /// <summary>
    /// Add operator. Each input quaternion's component is added to the corresponding quaternion's.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion that is Added.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator+(const QQuaternion &qQuat) const;

    /// <summary>
    /// Subtract operator. Each input quaternion's component is subtracted to the corresponding quaternion's.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion that is subtracted.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator-(const QQuaternion &qQuat) const;

    /// <summary>
    /// Multiply operator. The quaternion is multipled by the input one and the result is returned.
    /// This is calculated as follows:
    ///
    /// Q1 * Q2 = ( w1 w2 - x1 x2 - y1 y2 - z1 z2 ) +
    ///           ( w1 x2 + x1 w2 + y1 z2 - z1 y2 ) i +
    ///           ( w1 y2 + y1 w2 + z1 x2 - x1 z2 ) j +
    ///           ( w1 z2 + z1 w2 + x1 y2 - y1 x2 ) k.
    ///
    /// Note that quaternion multiplication is not conmutative.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to multiply by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator*(const QQuaternion &qQuat) const;

    /// <summary>
    /// Multiply by scalar operator. All quaternion's components are multiplied by the scalar.
    /// </summary>
    /// <param name="fScalar">[IN] The scalar to multily by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator*(const float_q &fScalar) const;

    /// <summary>
    /// Divide operator. The quaternion is divided by the input one. The division is performed by 
    /// multiplying by the input quaternion's inverse. Note that, if you want boost your division performance and
    /// you work with normalized quaternions, then you can multiply by the quaternion's conjugate (that is 
    /// cheaper to calculate) instead of using this operator.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to divide by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator/(const QQuaternion &qQuat) const;
    
    /// <summary>
    /// Divide by scalar operator. All quaternion's components are divided by the scalar.
    /// </summary>
    /// <param name="fScalar">[IN] The scalar to divide by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator/(const float_q &fScalar) const;

    /// <summary>
    /// Add and assign operator. Each input quaternion's component is added to the corresponding quaternion's.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion that is Added.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    inline QQuaternion& operator+=(const QQuaternion &qQuat)
    {
        this->x += qQuat.x;
        this->y += qQuat.y;
        this->z += qQuat.z;
        this->w += qQuat.w;
        return *this;
    }

    /// <summary>
    /// Subtract and assign operator. Each input quaternion's component is subtracted to the corresponding quaternion's.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion that is subtracted.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    inline QQuaternion& operator-=(const QQuaternion &qQuat)
    {
        this->x -= qQuat.x;
        this->y -= qQuat.y;
        this->z -= qQuat.z;
        this->w -= qQuat.w;

        return *this;
    }

    /// <summary>
    /// Multiply and assign operator. The quaternion is multipled by the input one.
    /// This is calculated as follows:
    ///
    /// Q1 * Q2 = ( w1 w2 - x1 x2 - y1 y2 - z1 z2 ) +
    ///           ( w1 x2 + x1 w2 + y1 z2 - z1 y2 ) i +
    ///           ( w1 y2 + y1 w2 + z1 x2 - x1 z2 ) j +
    ///           ( w1 z2 + z1 w2 + x1 y2 - y1 x2 ) k.
    ///
    /// Note that quaternion multiplication is not conmutative.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to multiply by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    inline QQuaternion& operator*=(const QQuaternion &qQuat)
    {
        QQuaternion resQuat( this->w * qQuat.x + this->x * qQuat.w + this->y * qQuat.z - this->z * qQuat.y,
                             this->w * qQuat.y + this->y * qQuat.w + this->z * qQuat.x - this->x * qQuat.z,
                             this->w * qQuat.z + this->z * qQuat.w + this->x * qQuat.y - this->y * qQuat.x,
                             this->w * qQuat.w - this->x * qQuat.x - this->y * qQuat.y - this->z * qQuat.z);
        this->x = resQuat.x;
        this->y = resQuat.y;
        this->z = resQuat.z;
        this->w = resQuat.w;
        
        return *this;
    }
    
    /// <summary>
    /// Multiply by scalar and assign operator. All quaternion's components are multiplied by the scalar.
    /// </summary>
    /// <param name="fScalar">[IN] The scalar to multiply by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    inline QQuaternion& operator*=(const float_q &fScalar)
    {
        this->x *= fScalar;
        this->y *= fScalar;
        this->z *= fScalar;
        this->w *= fScalar;

        return *this;
    }

    /// <summary>
    /// Divide and assign operator. The quaternion is divided by the input one. The division is performed by 
    /// multiplying by the input quaternion's inverse. Note that, if you want boost your division performance and
    /// you work with normalized quaternions, then you can multiply by the quaternion's conjugate (that is 
    /// cheaper to calculate) instead of using this operator.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to divide by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    inline QQuaternion& operator/=(const QQuaternion &qQuat)
    {
        QQuaternion resQuat;
        qQuat.Reverse(resQuat);
        this->operator*=(resQuat);

        return *this;
    }

    /// <summary>
    /// Divide by scalar and assign operator. All quaternion's components are divided by the scalar.
    /// </summary>
    /// <param name="fScalar">[IN] The scalar to divide by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    inline QQuaternion& operator/=(const float_q &fScalar)
    {
        QE_ASSERT(fScalar);

        this->x /= fScalar;
        this->y /= fScalar;
        this->z /= fScalar;
        this->w /= fScalar;

        return *this;
    }

    /// <summary>
    /// Equal operator. A tolerance value "Epsilon" is used for distincting whether the quaternions are equal or not.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to compare to.</param>
    /// <returns>
    /// If quaternions are equal, then it returns true. Otherwise, it returns false.
    /// </returns>
    inline bool operator==(const QQuaternion &qQuat) const
    {
        return   QFloat::AreEquals(this->x, qQuat.x) &&
                 QFloat::AreEquals(this->y, qQuat.y) &&
                 QFloat::AreEquals(this->z, qQuat.z) &&
                 QFloat::AreEquals(this->w, qQuat.w);
    }

    /// <summary>
    /// Non-equal operator. A tolerance value "Epsilon" is used for distincting whether the quaternions are equal or not.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to compare to.</param>
    /// <returns>
    /// If quaternions are not equal, then it returns true. Otherwise, it returns false.
    /// </returns>
    inline bool operator!=(const QQuaternion &qQuat) const
    {
        return QFloat::AreNotEquals(this->x, qQuat.x) ||
               QFloat::AreNotEquals(this->y, qQuat.y) ||
               QFloat::AreNotEquals(this->z, qQuat.z) ||
               QFloat::AreNotEquals(this->w, qQuat.w);
    }

    /// <summary>
    /// Normalizes the quaternion by dividing all quaternion's components by the quaternion's length.
    /// A quaternion is normalized when its length is equal to 1.
    /// </summary>
    inline void Normalize()
    {
        float_q fLength = this->GetLength();

        QE_ASSERT(fLength);

        this->x /= fLength;
        this->y /= fLength;
        this->z /= fLength;
        this->w /= fLength;
    }

    /// <summary>
    /// Gets a normalized copy of the quaternion. A quaternion is normalized when its length is equal to 1.
    /// To calculate it, all quaternion's components are divided by the quaternion's length.
    /// </summary>
    /// <param name="qQuat">[OUT] The normalized quaternion copy.</param>
    inline void Normalize(QBaseQuaternion &qQuat) const
    {
        float_q fLength = this->GetLength();

        QE_ASSERT(fLength);

        qQuat.x /= fLength;
        qQuat.y /= fLength;
        qQuat.z /= fLength;
        qQuat.w /= fLength;
    }

    /// <summary>
    /// Calculates the quaternion's inverse. Note that, when the quaternion is normalized, the inverse 
    /// coincides with the conjugate (which is cheaper to calculate).
    /// Quaternion inverse is obtained by the following equation:
    ///
    ///        w - xi - yj - zk
    /// Q^-1 = ----------------
    ///             |Q|^2
    /// </summary>
    inline void Reverse()
    {
        // [TODO] Thund: DirectX implementation uses ln(Q) = (0, theta * v), is it faster?
        float_q fSquaredLength = this->GetSquaredLength();

        QE_ASSERT(fSquaredLength);

        this->x = -this->x / fSquaredLength;
        this->y = -this->y / fSquaredLength;
        this->z = -this->z / fSquaredLength;
        this->w =  this->w / fSquaredLength;
    }

    /// <summary>
    /// Gets a reveted copy of the quaternion. Note that, when the quaternion is normalized, the inverse 
    /// coincides with the conjugate (which is cheaper to calculate).
    /// Quaternion inverse is obtained by the following equation:
    ///
    ///        w - xi - yj - zk
    /// Q^-1 = ----------------
    ///             |Q|^2
    /// </summary>
    /// <param name="qQuat">[OUT] The reverted quaternion copy.</param>
    inline void Reverse(QBaseQuaternion &qQuat) const
    {
        // [TODO] Thund: DirectX implementation uses ln(Q) = (0, theta * v), is it faster?
        float_q fSquaredLength = this->GetSquaredLength();

        QE_ASSERT(fSquaredLength);

        qQuat.x = -this->x / fSquaredLength;
        qQuat.y = -this->y / fSquaredLength;
        qQuat.z = -this->z / fSquaredLength;
        qQuat.w =  this->w / fSquaredLength;
    }

    /// <summary>
    /// Sets all quaternion's components to zero.
    /// </summary>
    inline void ResetToZero()
    {
        this->x = this->y = this->z = this->w = QFloat::_0;
    }

    /// <summary>
    /// Sets quaternion to the identity quaternion.
    /// </summary>
    inline void ResetToIdentity()
    {
        this->x = this->y = this->z = QFloat::_0;
        this->w = QFloat::_1;
    }

    /// <summary>
    /// Calculates the dot product between the quaternion and the input quaternion.
    /// This is obtained using the equation: f(Q1, Q2) = x1 x2 + y1 y2 + z1 z2 + w1 w2.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to multiply by.</param>
    /// <returns>
    /// A real number equals to: |Q1| |Q2| cos(B), where B = half the angle between quaternions, when using unit quaternions.
    /// </returns>
    inline float_q DotProduct(const QBaseQuaternion &qQuat) const
    {
        return this->x * qQuat.x + this->y * qQuat.y + this->z * qQuat.z + this->w * qQuat.w;
    }

    /// <summary>
    /// Calculates the quaternion's conjugate. It's calculated this way: Q* = w - xi - yj - zk.
    /// </summary>
    inline void Conjugate()
    {
        this->x = -this->x;
        this->y = -this->y;
        this->z = -this->z;
        this->w =  this->w;
    }

    /// <summary>
    /// Gets a conjugated quaternion copy. It's calculated this way: Q* = w - xi - yj - zk.
    /// </summary>
    /// <param name="qQuat">[OUT] The conjugated quaternion copy.</param>
    inline void Conjugate(QBaseQuaternion &qQuat) const
    {
        qQuat.x = -this->x;
        qQuat.y = -this->y;
        qQuat.z = -this->z;
        qQuat.w =  this->w;
    }

    /// <summary>
    /// Calculates the linear interpolation between the quaternion and the input quaternion. This is calculated
    /// by the following expression:
    ///
    ///                  (1 - s)Q1 + sQ2
    /// f(Q1, Q2, s) = -------------------
    ///                 |(1 - s)Q1 + sQ2|
    ///
    /// being Q1 and Q2 two quaternions and s the scalar proportion of distance from Q1 to Q2.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to interpolate with (Q2 in expression above).</param>
    /// <param name="fProportion">[IN] The scalar proportion of distance from Q1 to Q2.</param>
    void Lerp(const QQuaternion &qQuat, const float_q &fProportion);

    /// <summary>
    /// Calculates the linear interpolation between the quaternion and the input quaternion. This is calculated
    /// by the following expression:
    ///
    ///                  (1 - s)Q1 + sQ2
    /// f(Q1, Q2, s) = -------------------
    ///                 |(1 - s)Q1 + sQ2|
    ///
    /// being Q1 and Q2 two quaternions and s the scalar proportion of distance from Q1 to Q2.
    /// The resultant quaternion is stored in an output quaternion.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to interpolate with (Q2 in expression above).</param>
    /// <param name="fProportion">[IN] The scalar proportion of distance from Q1 to Q2.</param>
    /// <param name="qOutQuat">[OUT] The resultant quaternion.</param>
    void Lerp(const QQuaternion &qQuat, const float_q &fProportion, QQuaternion &qOutQuat) const;

    /// <summary>
    /// Calculates the spherical linear interpolation between the quaternion and the input quaternion. This is
    /// calculated by the following expression:
    ///
    /// f(Q1, Q2, s) = w1 Q1 + w2 Q2
    ///
    /// where
    ///       sin( (1 - s) * B )
    /// w1 = --------------------
    ///             sin(B)
    ///
    ///       sin(sB)
    /// w2 = ---------
    ///        sin(B)
    ///
    /// where
    ///
    /// B = arccos(Q1 � Q2)
    ///
    /// being Q1 and Q2 two quaternions and s the scalar proportion of distance from Q1 to Q2.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to interpolate with (Q2 in expression above).</param>
    /// <param name="fProportion">[IN] The scalar proportion of distance from Q1 to Q2.</param>
    void Slerp(const QQuaternion &qQuat, const float_q &fProportion);
    
    /// <summary>
    /// Calculates the spherical linear interpolation between the quaternion and the input quaternion. This is
    /// calculated by the following expression:
    ///
    /// f(Q1, Q2, s) = w1 Q1 + w2 Q2
    ///
    /// where
    ///       sin( (1 - s) * B )
    /// w1 = --------------------
    ///             sin(B)
    ///
    ///       sin(sB)
    /// w2 = ---------
    ///        sin(B)
    ///
    /// where
    ///
    /// B = arccos(Q1 � Q2)
    ///
    /// being Q1 and Q2 two quaternions and s the scalar proportion of distance from Q1 to Q2.
    /// The resultant quaternion is stored in an output quaternion.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to interpolate with (Q2 in expression above).</param>
    /// <param name="fProportion">[IN] The scalar proportion of distance from Q1 to Q2.</param>
    void Slerp(const QQuaternion &qQuat, const float_q &fProportion, QQuaternion &qOutQuat) const;
    
    /// <summary>
    /// Obtains Euler angles that represent the same rotation than the quaternion does.
    /// Quimera Engine follows the rotation order convention: Z, then X, then Y, aka Yaw-Pitch-Roll.
    /// To achieve this, the following equations are implemented:
    ///
    /// X = atan2( 2xw - 2xz, 1 - 2y^2 - 2z^2 )
    /// Y = asin(  2*xy + 2zw)
    /// Z = atan2( 2xw - 2yz, 1 - 2x^2 - 2z^2 )
    ///
    /// except when xy + zw = +0.5 (north pole)
    ///
    /// X = 2atan2(x, w)
    /// Z = 0
    ///
    /// or when xy + zw = -0.5 (south pole)
    ///
    /// X = -2atan2(x, w)
    /// Z = 0
    ///
    /// See atan2 documentation for more interesting information.
    /// </summary>
    /// <param name="fAngleX">X Euler angle (pitch).</param>
    /// <param name="fAngleY">Y Euler angle (yaw).</param>
    /// <param name="fAngleZ">Z Euler angle (roll).</param>
    inline void ToEulerAngles(float_q &fAngleX, float_q &fAngleY, float_q &fAngleZ) const
    {
        // [TODO] Thund: Check whether this implementation corresponds to YawPitchRoll.
        const float_q fNorthAndSouthPoleCheckValue = this->x * this->y + this->z * this->w;

        // X (pitch or heading) and Z (roll or bank) checking
        if(fNorthAndSouthPoleCheckValue == QFloat::_0_5) // North pole
        {
            // The atan2 result is undefined when both parameters are equal to zero
            QE_ASSERT(this->x || this->w);

            fAngleX = QFloat::_2 * atan2(this->x, this->w);
            fAngleZ = QFloat::_0;
        }
        else if(fNorthAndSouthPoleCheckValue == -QFloat::_0_5) // South pole
        {
            // The atan2 result is undefined when both parameters are equal to zero
            QE_ASSERT(this->x || this->w);

            fAngleX = -QFloat::_2 * atan2(this->x, this->w);
            fAngleZ = QFloat::_0;
        }
        else
        {
            float_q fFirstParameter  = QFloat::_2 * this->x * this->w - QFloat::_2 * this->x * this->z;
            float_q fSecondParameter = QFloat::_1 - QFloat::_2 * this->y * this->y - QFloat::_2 * this->z * this->z;

            // The atan2 result is undefined when both parameters are equal to zero
            QE_ASSERT(fFirstParameter || fSecondParameter);

            fAngleX = atan2(fFirstParameter, fSecondParameter);

            fFirstParameter = QFloat::_2 * this->x * this->w - QFloat::_2 * this->y * this->z;
            fSecondParameter = QFloat::_1 - QFloat::_2 * this->x * this->x - QFloat::_2 * this->z * this->z;

            // The atan2 result is undefined when both parameters are equal to zero
            QE_ASSERT(fFirstParameter || fSecondParameter);

            fAngleZ = atan2(fFirstParameter, fSecondParameter);
        }

        fAngleY = asin( QFloat::_2 * this->x * this->y + QFloat::_2 * this->z * this->w );

        QE_ASSERT( !QFloat::IsNaN(fAngleX) );
        QE_ASSERT( !QFloat::IsNaN(fAngleY) );
        QE_ASSERT( !QFloat::IsNaN(fAngleZ) );

    }

    /// <summary>
    /// Gets a character string that represents the quaternion values.
    /// </summary>
    /// <returns>
    /// A character string, following this pattern: "Q( x, y, z, w)".
    /// </returns>
    inline std::string ToString() const;

    /// <summary>
    /// Calculates the quaternion's length, this means, the square root of the squared components sum.
    /// </summary>
    /// <returns>
    /// The resultant length.
    /// </returns>
    inline float_q GetLength() const
    {
        //           _______________________
        // length = V x^2 + y^2 + z^2 + w^2
        //
        return sqrt( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w));
    }
    
    /// <summary>
    /// Calculates the squared quaternion's length, this means, the squared components sum. This method
    /// exists due to performance reasons because it avoids to perform square roots when the result is 
    /// going to be squared.
    /// </summary>
    /// <returns>
    /// The resultant squared length.
    /// </returns>
    inline float_q GetSquaredLength() const
    {
        //                    ______________________
        // squared length = (V x^2 + y^2 + z^2 + w^2)^2
        //
        return (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w);
    }

};


} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QQUATERNION__