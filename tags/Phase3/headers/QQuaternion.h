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

#ifndef __QQUATERNION__
#define __QQUATERNION__

#include "QBaseQuaternion.h"
#include "StringsDefinitions.h"

using Kinesis::QuimeraEngine::Common::DataTypes::float_q;
using Kinesis::QuimeraEngine::Common::DataTypes::vf32_q;
using Kinesis::QuimeraEngine::Common::DataTypes::string_q;


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Math
{

// Forward declarations
class QMatrix4x3;
class QMatrix4x4;
template<class MatrixT> class QTransformationMatrix;
class QRotationMatrix3x3;
class QBaseVector3;
class QBaseVector4;


/// <summary>
/// A Quaternion is defined using four floating point values |X Y Z W|. 
/// These are calculated from the combination of the three coordinates
/// of the rotation axis and the rotation angle.
/// </summary>
/// <remarks>
/// Quaternions extend the concept of rotation in three dimensions to rotation in four dimensions.
/// This avoids the problem of "gimbal-lock"
/// and allows for the implementation of smooth and continuous rotation.<br/>
/// In effect, they may be considered to add a additional rotation angle
/// to spherical coordinates ie. Longitude, Latitude and Rotation angles.<br/>
/// <br/>
/// (Text extracted from: Flipcode.)
/// </remarks>
class QE_LAYER_TOOLS_SYMBOLS QQuaternion : public QBaseQuaternion
{
    // FRIENDS
    // ---------------
public:

    /// <summary>
    /// Multiply by scalar operator. All quaternion's components are multiplied by the scalar.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="fScalar">[IN] The scalar factor.</param>
    /// <param name="qQuat">[IN] The quaternion factor.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    friend QQuaternion QE_LAYER_TOOLS_SYMBOLS operator*(const float_q fScalar, const QQuaternion &qQuat);


    // CONSTRUCTORS
    // ---------------
public:

    /// <summary>
    /// Default constructor.
    /// </summary>
    QQuaternion();

    /// <summary>
    /// Copy constructor.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion from which we want to create a copy in the resident quaternion.</param>
    QQuaternion(const QQuaternion &qQuat);

    /// <summary>
    /// Base type constructor.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion in which we want resident quaternion to be based.</param>
    QQuaternion(const QBaseQuaternion &qQuat);

    /// <summary>
    /// Constructor that receives 3 angles, one for each Euler angle, to represent a spacial rotation as a quaternion.
    /// </summary>
    /// <remarks>
    /// Quimera Engine follows the rotation order convention: Z, then X, then Y, aka Yaw-Pitch-Roll.<br/>
    /// This is a slow operation.<br/>
    /// This method produces a normalized quaternion.
    /// </remarks>
    /// <param name="fRotationAngleX">[IN] Rotation angle about X global axis.</param>
    /// <param name="fRotationAngleY">[IN] Rotation angle about Y global axis.</param>
    /// <param name="fRotationAngleZ">[IN] Rotation angle about Z global axis.</param>
    QQuaternion(const float_q fRotationAngleX, const float_q fRotationAngleY, const float_q fRotationAngleZ);

    /// <summary>
    /// Constructor that receives 4 values, one per quaternion's component.
    /// </summary>
    /// <param name="fValueX">[IN] X component value.</param>
    /// <param name="fValueY">[IN] Y component value.</param>
    /// <param name="fValueZ">[IN] Z component value.</param>
    /// <param name="fValueW">[IN] W component value.</param>
    QQuaternion(const float_q fValueX, const float_q fValueY, const float_q fValueZ, const float_q fValueW);

    /// <summary>
    /// Constructor that receives a pointer to a sequence of 4 contiguous values, one per quaternion's component in the order: X, Y, Z and W.
    /// </summary>
    /// <param name="arValues">[IN] Sequence of 4 contiguous values. If the pointer is null, the behavior  
    /// is undefined.</param>
    explicit QQuaternion(const float_q* arValues);

    /// <summary>
    /// Constructor that receives four 32 bits floating point type, one per quaternion's component, packaged
    /// into a 128 bits structure.<br/>
    /// The values order is: X, Y, Z and W.
    /// </summary>
    /// <param name="value">[IN] A four 32 bits floating point types pack.</param>
    explicit QQuaternion(const vf32_q value);

    /// <summary>
    /// Constructor that receives a rotation angle and a normalized vector in the direction of the spin axis.
    /// </summary>
    /// <remarks>
    /// Being \f$ (\theta)\f$ the angle and \f$ \vec{n}(n_x, n_y, n_z)\f$ the axis, he resultant quaternion is:
    ///
    /// \f$ (n_x sin(\frac{\theta}{2}), n_y sin(\frac{\theta}{2}), n_z sin(\frac{\theta}{2}), cos(\frac{\theta}{2}))\f$ <br/>
    /// Please note it's a mandatory that the input vector representing the spin axis has to be normalized
    /// in order to create the quaternion correctly (a normalized rotation quaternion).
    /// </remarks>
    /// <param name="vRotationAxis">[IN] Normalined vector in the direction of the spin axis.</param>
    /// <param name="fRotationAngle">[IN] Angle of rotation.</param>
    QQuaternion(const QBaseVector3 &vRotationAxis, const float_q fRotationAngle);

    /// <summary>
    /// Constructor that receives a rotation angle and a normalized vector in the direction of the spin axis.
    /// </summary>
    /// <remarks>
    /// Being \f$ (\theta)\f$ the angle and \f$ \vec{n}(n_x, n_y, n_z)\f$ the axis, he resultant quaternion is:
    ///
    /// \f$ (n_x sin(\frac{\theta}{2}), n_y sin(\frac{\theta}{2}), n_z sin(\frac{\theta}{2}), cos(\frac{\theta}{2}))\f$ <br/>
    /// Please note it's a mandatory that the input vector representing the spin axis has to be normalized
    /// in order to create the quaternion correctly (a normalized rotation quaternion).
    /// </remarks>
    /// <param name="vRotationAxis">[IN] Normalized vector in the direction of the spin axis.</param>
    /// <param name="fRotationAngle">[IN] Angle of rotation.</param>
    QQuaternion(const QBaseVector4 &vRotationAxis, const float_q fRotationAngle);

    /// <summary>
    /// Constructor that receives a transformation matrix. The quaternion will contain the rotation the matrix represents.
    /// </summary>
    /// <remarks>
    /// Realize that getting the rotation of transformation matrices which were compound of negative scales may
    /// cause that obtained quaternion doesn't match the one used to build the matrix.<br/>
    /// This method produces a normalized quaternion.
    /// </remarks>
    /// <param name="transformation">[IN] A transformation matrix. If it is a null matrix, the result is undefined.</param>
    explicit QQuaternion(const QTransformationMatrix<QMatrix4x3> &transformation);

    /// <summary>
    /// Constructor that receives a transformation matrix. The quaternion will contain the rotation the matrix represents.
    /// </summary>
    /// <remarks>
    /// Realize that getting the rotation of transformation matrices which were compound of negative scales may
    /// cause that obtained quaternion doesn't match the one used to build the matrix.<br/>
    /// This method produces a normalized quaternion.
    /// </remarks>
    /// <param name="transformation">[IN] A transformation matrix. If it is a null matrix, the result is undefined.</param>
    explicit QQuaternion(const QTransformationMatrix<QMatrix4x4> &transformation);

    /// <summary>
    /// Constructor that receives a 3x3 rotation matrix.
    /// </summary>
    /// <remarks>
    /// This method produces a normalized quaternion.
    /// </remarks>
    /// <param name="rotation">[IN] A 3x3 rotation matrix. If it is a null matrix, the result is undefined.</param>
    explicit QQuaternion(const QRotationMatrix3x3 &rotation);

private:

    /// <summary>
    /// Constructor that receives a transformation matrix. The quaternion will contain the rotation the matrix represents.
    /// </summary>
    /// <typeparam name="MatrixT">Allowed types: QMatrix4x3, QMatrix4x4.</typeparam>
    /// <param name="transformation">[IN] A transformation matrix. If it is a null matrix, the result is undefined.</param>
    template <class MatrixT>
    void QQuaternionImp(const QTransformationMatrix<MatrixT> &transformation);


    // PROPERTIES
    // ---------------
public:

    /// <summary>
    /// Gets the identity quaternion.
    /// </summary>
    /// <returns>
    /// The identity quaternion.
    /// </returns>
    static const QQuaternion& GetIdentity();


    // METHODS
    // ---------------
public:

    /// <summary>
    /// Add operator. Each input quaternion's component is added to the corresponding quaternion's.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion that is Added.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator+(const QBaseQuaternion &qQuat) const;

    /// <summary>
    /// Subtract operator. Each input quaternion's component is subtracted to the corresponding quaternion's.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion that is subtracted.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator-(const QBaseQuaternion &qQuat) const;

    /// <summary>
    /// Multiply operator. The quaternion is multipled by the input one and the result is returned.
    /// </summary>
    /// <remarks>
    /// This is calculated as follows:<br/>
    /// <br/>
    /// \f$ Q_1 \cdot Q_2=(w_1w_2-x_1x_2-y_1y_2-z_1z_2)+(w_1x_2+x_1w_2+y_1z_2-z_1y_2)i+(w_1y_2+y_1w_2+z_1x_2-x_1z_2)j+(w_1z_2+z_1w_2+xv1y_2-y_1x_2)k\f$<br/>
    /// <br/>
    /// Note that quaternion multiplication is not conmutative.<br/>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion to multiply by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator*(const QBaseQuaternion &qQuat) const;// [DOC] Thund: Update the documentation (formula changed)

    /// <summary>
    /// Multiply by scalar operator. All quaternion's components are multiplied by the scalar.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="fScalar">[IN] The scalar to multiply by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator*(const float_q fScalar) const;

    /// <summary>
    /// Multiply by 3D vector operator.<br/>
    /// The vector is converted into a quaternion \f$ (v_x, v_y, v_z, 0) \f$ before multiplication.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="vVector">[IN] The vector to multiply by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator*(const QBaseVector3 &vVector) const;

    /// <summary>
    /// Multiply by 4D vector operator.<br/>
    /// The vector is converted into a quaternion \f$ (v_x, v_y, v_z, v_w) \f$ before multiplication.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="vVector">[IN] The vector to multiply by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator*(const QBaseVector4 &vVector) const;

    /// <summary>
    /// Division operator. The quaternion is divided by the input one.
    /// </summary>
    /// <remarks>
    /// The division is performed by multiplying by the input quaternion's inverse.<br/>
    /// Note that, if you want boost your division performance and you work with normalized quaternions,
    /// then you can multiply by the quaternion's conjugate (that is
    /// cheaper to calculate) instead of using this operator.<br/>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion to divide by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator/(const QBaseQuaternion &qQuat) const;

    /// <summary>
    /// Divide by scalar operator. All quaternion's components are divided by the scalar.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="fScalar">[IN] The scalar to divide by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion operator/(const float_q fScalar) const;

    /// <summary>
    /// Add and assign operator. Each input quaternion's component is added to the corresponding quaternion's.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion that is Added.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    QQuaternion& operator+=(const QBaseQuaternion &qQuat);

    /// <summary>
    /// Subtract and assign operator. Each input quaternion's component is subtracted to the corresponding quaternion's.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion that is subtracted.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    QQuaternion& operator-=(const QBaseQuaternion &qQuat);

    /// <summary>
    /// Multiply and assign operator. The quaternion is multipled by the input one.
    /// </summary>
    /// <remarks>
    /// This is calculated as follows, being 1:<br/>
    /// <br/>
    /// \f$ Q_1 \cdot Q_2=(w_1x_2+x_1w_2+y_1z_2-z_1y_2)+(w_1y_2+y_1w_2+z_1x_2-x_1z_2)i+(w_1z_2+z_1w_2+z_1y_2-y_1z_2)j+(w_1w_2-x_1x_2-y_1y_2-z_1z_2)k\f$<br/>
    /// <br/>
    /// Note that quaternion multiplication is not conmutative.<br/>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion to multiply by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    QQuaternion& operator*=(const QBaseQuaternion &qQuat);

    /// <summary>
    /// Multiply by scalar and assign operator. All quaternion's components are multiplied by the scalar.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="fScalar">[IN] The scalar to multiply by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    QQuaternion& operator*=(const float_q fScalar);

    /// <summary>
    /// Multiply by 3D vector and assign operator.<br/>
    /// The vector is converted into a quaternion \f$ (v_x, v_y, v_z, 0) \f$ before multiplication.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="vVector">[IN] The vector to multiply by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    QQuaternion& operator*=(const QBaseVector3 &vVector);

    /// <summary>
    /// Multiply by 4D vector and assign operator.<br/>
    /// The vector is converted into a quaternion \f$ (v_x, v_y, v_z, v_w) \f$ before multiplication.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="vVector">[IN] The vector to multiply by.</param>
    /// <returns>
    /// The resultant quaternion.
    /// </returns>
    QQuaternion& operator*=(const QBaseVector4 &vVector);

    /// <summary>
    /// Division and assign operator. The quaternion is divided by the input one.
    /// </summary>
    /// <remarks>
    /// The division is performed by multiplying by the input quaternion's inverse.<br/>
    /// Note that, if you want boost your division performance and
    /// you work with normalized quaternions, then you can multiply by the quaternion's conjugate (that is
    /// cheaper to calculate) instead of using this operator.<br/>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion to divide by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    QQuaternion& operator/=(const QBaseQuaternion &qQuat);

    /// <summary>
    /// Divide by scalar and assign operator. All quaternion's components are divided by the scalar.
    /// </summary>
    /// <remarks>
    /// Note that the quaternion could be denormalized after this operation.
    /// </remarks>
    /// <param name="fScalar">[IN] The scalar to divide by.</param>
    /// <returns>
    /// The modified quaternion.
    /// </returns>
    QQuaternion& operator/=(const float_q fScalar);

    /// <summary>
    /// Assignation operator. Assigns the provided quaternion to the resident quaternion.
    /// </summary>
    /// <param name="qQuat">[IN] The quaternion to be assigned.</param>
    /// <returns>
    /// A reference to the modified quaternion.
    /// </returns>
    QQuaternion& operator=(const QBaseQuaternion &qQuat);

    /// <summary>
    /// Unary minus operator. Obtains a copy of the resident quaternion which has each component multiplied by -1.
    /// </summary>
    /// <returns>
    /// A copy of the resident quaternion which has each component multiplied by -1.
    /// </returns>
    QQuaternion operator-() const;

    /// <summary>
    /// Normalizes the quaternion by dividing all quaternion's components by the quaternion's length.
    /// </summary>
    /// <remarks>
    /// A quaternion is normalized when its length is equal to 1.<br/>
    /// If the length of the quaternion equals zero, the result is undefined.
    /// </remarks>
    /// <returns>
    /// The normalized quaternion.
    /// </returns>
    QQuaternion Normalize() const;

    /// <summary>
    /// Calculates the quaternion's inverse.
    /// </summary>
    /// <remarks>
    /// Note that, when the quaternion is normalized, the inverse coincides with the conjugate (which is cheaper to calculate).<br/>
    /// Quaternion inverse is obtained by the following equation:<br/>
    /// <br/>
    /// \f$ Q^{-1} = \frac{w - xi - yj - zk}{\left|Q\right|^2}\f$
    /// </remarks>
    /// <returns>
    /// The reversed quaternion.
    /// </returns>
    QQuaternion Invert() const;

    /// <summary>
    /// Calculates the inverse of a normalized quaternion, which coincides with its conjugate.
    /// </summary>
    /// <remarks>
    /// Quaternion inverse is then obtained by the following equation:<br/>
    /// <br/>
    /// \f$ Q^{-1} = w - xi - yj - zk \f$
    /// </remarks>
    /// <returns>
    /// The reversed quaternion.
    /// </returns>
    QQuaternion UnitInvert() const;

    /// <summary>
    /// Sets all quaternion's components to zero.
    /// </summary>
    void ResetToZero();

    /// <summary>
    /// Sets quaternion to the identity quaternion.
    /// </summary>
    void ResetToIdentity();

    /// <summary>
    /// Calculates the dot product between the quaternion and the input quaternion.
    /// </summary>
    /// <remarks>
    /// This is obtained using the equation:<br/>
    /// <br/>
    /// \f$ f(Q_1, Q_2) = x_1x_2 + y_1y_2 + z_1z_2 + w_1w_2\f$.
    /// </remarks>
    /// <param name="qQuat">[IN] The quaternion to multiply by.</param>
    /// <returns>
    /// A real number equals to:<br/>
    /// <br/>
    /// \f$|Q_1|\cdot |Q_2| cos(\beta)\f$, where<br/>
    /// <br/>
    /// \f$\beta = \frac{\widehat{Q_1Q_2}}{2}\f$<br/>
    /// <br/>
    /// (half the angle between quaternions, when using normalized quaternions).
    /// </returns>
    float_q DotProduct(const QBaseQuaternion &qQuat) const;

    /// <summary>
    /// Calculates the angle between resident quaternion and the provided quaternion, via dot product.
    /// </summary>
    /// <remarks>
    /// Both quaternions have to be normalized to obtain a more precise value.<br/>
    /// The result could be not as accurate as expected due to loss of precision when working with angles using floating point numbers.
    /// </remarks>
    /// <param name="qQuat">[IN] Multiplying quaternion.</param>
    /// <returns>
    /// A floating point value which is the smaller angle between quaternions (less or equal to \f$ 180^0\f$).
    /// </returns>
    float_q AngleBetween(const QQuaternion &qQuat) const;

    /// <summary>
    /// Calculates the quaternion's conjugate.
    /// </summary>
    /// <remarks>
    /// It's calculated this way:<br/>
    /// <br/>
    /// \f$Q^* = w - xi - yj - zk\f$.
    /// </remarks>
    /// <returns>
    /// The conjugated quaternion.
    /// </returns>
    QQuaternion Conjugate() const;

    /// <summary>
    /// Calculates the linear interpolation between the quaternion and the input quaternion.
    /// </summary>
    /// <remarks>
    /// This is calculated by the following expression:<br/>
    /// <br/>
    /// \f$ f(Q_1, Q_2, s) = \frac{(1 - s)Q_1 + sQ_2}{|(1 - s)Q_1 + sQ_2|}\f$<br/>
    /// <br/>
    /// being \f$ Q_1\f$ and \f$ Q_2\f$ two quaternions and \f$ s\f$ the scalar proportion of distance from \f$ Q_1\f$ to \f$ Q_2\f$.
    /// </remarks>
    /// <param name="fProportion">[IN] The scalar proportion of distance from \f$ Q_1\f$ to \f$ Q_2\f$. The closer to 1, the closer to the input quaternion; 
    /// the closer to zero, the closer to the resident quaternion.</param>
    /// <param name="qQuat">[IN] The quaternion to interpolate with (\f$ Q_2\f$ in expression above).</param>
    /// <returns>
    /// The "lerped" quaternion. It's normalized.
    /// </returns>
    QQuaternion Lerp(const float_q fProportion, const QQuaternion &qQuat) const;

    /// <summary>
    /// Calculates the spherical linear interpolation between this quaternion and the input quaternion.
    /// </summary>
    /// <remarks>
    /// This is calculated by the following expression:<br/>
    /// <br/>
    /// \f$ f(Q_1, Q_2, s) = w_1Q_1 + w_2Q_2\f$<br/>
    /// <br/>
    /// where<br/>
    /// <br/>
    /// \f$ w_1 = \frac{sin( (1 - s) \beta)}{sin(\beta)}\f$<br/>
    /// <br/>
    /// \f$ w_2 = \frac{sin( s\beta)}{sin(\beta)})\f$<br/>
    /// <br/>
    /// where<br/>
    /// <br/>
    /// \f$ \beta = \arccos(Q_1Q_2)\f$<br/>
    /// <br/>
    /// being \f$ Q_1\f$ and \f$ Q_2\f$ two quaternions and \f$ s\f$ the scalar proportion of distance from \f$ Q_1\f$ to \f$ Q_2\f$.
    /// </remarks>
    /// <param name="fProportion">[IN] The scalar proportion of distance from \f$ Q_1\f$ to \f$ Q_2\f$. The closer to 1, the closer to the input quaternion; 
    /// the closer to zero, the closer to the resident quaternion.</param>
    /// <param name="qQuat">[IN] The quaternion to interpolate with (\f$ Q_2\f$ in expression above).</param>
    /// <returns>
    /// The "lerped" quaternion. If \f$ \beta = 0\f$ or \f$ \beta = \pi\f$, the returned value is the resident quaternion.
    /// </returns>
    QQuaternion Slerp(const float_q fProportion, const QQuaternion &qQuat) const;

    /// <summary>
    /// Calculates the spherical linear interpolation between two normalized quaternions.
    /// </summary>
    /// <remarks>
    /// This is calculated by the following expression:<br/>
    /// <br/>
    /// \f$ f(Q_1, Q_2, s) = w_1Q_1 + w_2Q_2\f$<br/>
    /// <br/>
    /// where<br/>
    /// <br/>
    /// \f$ w_1 = \frac{sin( (1 - s) \beta)}{sin(\beta)}\f$<br/>
    /// <br/>
    /// \f$ w_2 = \frac{sin( s\beta)}{sin(\beta)})\f$<br/>
    /// <br/>
    /// where<br/>
    /// <br/>
    /// \f$ \beta = \arccos(Q_1Q_2)\f$<br/>
    /// <br/>
    /// being \f$ Q_1\f$ and \f$ Q_2\f$ two normalized quaternions and \f$ s\f$ the scalar proportion of distance from \f$ Q_1\f$ to \f$ Q_2\f$.
    /// </remarks>
    /// <param name="fProportion">[IN] The scalar proportion of distance from \f$ Q_1\f$ to \f$ Q_2\f$. The closer to 1, the closer to the input quaternion; 
    /// the closer to zero, the closer to the resident quaternion.</param>
    /// <param name="qQuat">[IN] The quaternion to interpolate with (\f$ Q_2\f$ in expression above). It must be a normalized quaternion</param>
    /// <returns>
    /// The "lerped" quaternion. If \f$ \beta = 0\f$ or \f$ \beta = \pi\f$ (see formulas), the returned value is the resident quaternion.
    /// </returns>
    QQuaternion UnitSlerp(const float_q fProportion, const QQuaternion &qQuat) const;

    /// <summary>
    /// Obtains Euler angles that represent the same rotation than the quaternion does.<br/>
    /// </summary>
    /// <remarks>
    /// The quaternion must be normalized in order to get correct results.<br/>
    /// Quimera Engine follows the rotation order convention: Z, then X, then Y, aka Yaw-Pitch-Roll.<br/>
    /// To achieve this, the following equations are implemented:<br/>
    /// <br/>
    /// \f$ X = asin( 2xw - 2zy )\f$<br/>
    /// <br/>
    /// \f$ Y = atan2( 2wz + 2xy, 1 - 2z^2 + 2y^2 )\f$<br/>
    /// <br/>
    /// \f$ Z = atan2( 2wz + 2xy, 1 - 2z^2 - 2x^2 )\f$<br/>
    /// <br/>
    /// except when \f$ X = \pm\frac{\pi}{2}\f$ (north or south pole)<br/>
    /// <br/>
    /// \f$ Y = 0\f$<br/>
    /// <br/>
    /// \f$ Z = atan2(z, w)\f$<br/>
    /// <br/>
    /// See atan2 documentation for more interesting information.<br/>
    /// Note that obtained angles may not match whatever values
    /// were used to create the quaternion from 3 Euler angles.
    /// </remarks>
    /// <param name="fRotationAngleX">X Euler angle (pitch).</param>
    /// <param name="fRotationAngleY">Y Euler angle (yaw).</param>
    /// <param name="fRotationAngleZ">Z Euler angle (roll).</param>
    void ToEulerAngles(float_q &fRotationAngleX, float_q &fRotationAngleY, float_q &fRotationAngleZ) const;
    
    /// <summary>
    /// Calculates the quaternion's length, this means, the square root of the squared components sum.
    /// </summary>
    /// <returns>
    /// The resultant length.
    /// </returns>
    float_q GetLength() const;

    /// <summary>
    /// Calculates the squared quaternion's length, this means, the squared components sum.
    /// </summary>
    /// <remarks>
    /// This method exists due to performance reasons because it avoids to perform square roots when the result is
    /// going to be squared.
    /// </remarks>
    /// <returns>
    /// The resultant squared length.
    /// </returns>
    float_q GetSquaredLength() const;

    /// <summary>
    /// Obtains the angle of rotation and the spin axis contained in the resident quaternion.
    /// </summary>
    /// <remarks>
    /// There are two possible singularities: when rotation angle is 0 or \f$ 180^0\f$.
    /// </remarks>
    /// <param name="vRotationAxis">Vector to store the spin axis.</param>
    /// <param name="fRotationAngle">Angle of rotation.</param>
    void ToAxisAngle(QBaseVector3 &vRotationAxis, float_q &fRotationAngle) const;

    /// <summary>
    /// Obtains the angle of rotation and the spin axis contained in the resident quaternion.
    /// </summary>
    /// <remarks>
    /// There are two possible singularities: when rotation angle is 0 or \f$ 180^0\f$.<br/>
    /// Note that angle and axis haven't to match whatever values were used to create the quaternion from
    /// an axis and an angle.
    /// </remarks>
    /// <param name="vRotationAxis">Vector to store the spin axis.</param>
    /// <param name="fRotationAngle">Angle of rotation.</param>
    void ToAxisAngle(QBaseVector4 &vRotationAxis, float_q &fRotationAngle) const;

    /// <summary>
    /// Gets a character string that represents the quaternion values.
    /// </summary>
    /// <remarks>
    /// A character string, following this pattern:<br/>
    /// "Q($x,$y,$z,$w)".<br/>
    /// Where "$" means "string representation of attribute".
    /// </remarks>
    /// <returns>
    /// The string with the format specified.
    /// </returns>
    string_q ToString() const;
};

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QQUATERNION__
