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

#include "QRotationMatrix3x3.h"
#include "SQAngle.h"
#include "QVector3.h"
#include "QQuaternion.h"
#include "QTransformationMatrix.h"
#include "QScalingMatrix3x3.h"
#include "QTranslationMatrix.h"
#include "QMatrix4x4.h"
#include "QMatrix4x3.h"

using Kinesis::QuimeraEngine::Common::DataTypes::float_q;
using Kinesis::QuimeraEngine::Common::DataTypes::SQFloat;
using Kinesis::QuimeraEngine::Tools::Math::QRotationMatrix3x3;

#if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES && QE_CONFIG_PRECISION_DEFAULT == QE_CONFIG_PRECISION_SIMPLE
    const float_q SMALLER_TOLERANCE = 1e-5f;
#else
    const float_q SMALLER_TOLERANCE = SQFloat::Epsilon;
#endif

QTEST_SUITE_BEGIN( QRotationMatrix3x3_TestSuite )

/// <summary>
/// Checks if default values have changed.
/// </summary>
QTEST_CASE ( Constructor1_DefaultValuesHaventChanged_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QMatrix3x3;

    const QRotationMatrix3x3 EXPECTED_VALUE(QRotationMatrix3x3::GetIdentity());

    // [Execution]
    QRotationMatrix3x3 matrixUT;

    // [Verification]
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks if copy constructor copies every matrix element properly.
/// </summary>
QTEST_CASE ( Constructor2_EveryMatrixElementCopiedProperly_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const float_q EXPECTED_VALUE_FOR_00 = SQFloat::_1;
    const float_q EXPECTED_VALUE_FOR_01 = SQFloat::_2;
    const float_q EXPECTED_VALUE_FOR_02 = SQFloat::_3;
    const float_q EXPECTED_VALUE_FOR_10 = SQFloat::_4;
    const float_q EXPECTED_VALUE_FOR_11 = SQFloat::_5;
    const float_q EXPECTED_VALUE_FOR_12 = SQFloat::_6;
    const float_q EXPECTED_VALUE_FOR_20 = SQFloat::_7;
    const float_q EXPECTED_VALUE_FOR_21 = SQFloat::_8;
    const float_q EXPECTED_VALUE_FOR_22 = SQFloat::_9;

    const QBaseMatrix3x3 EXPECTED_VALUE(EXPECTED_VALUE_FOR_00, EXPECTED_VALUE_FOR_01, EXPECTED_VALUE_FOR_02,
                                        EXPECTED_VALUE_FOR_10, EXPECTED_VALUE_FOR_11, EXPECTED_VALUE_FOR_12,
                                        EXPECTED_VALUE_FOR_20, EXPECTED_VALUE_FOR_21, EXPECTED_VALUE_FOR_22);

    const QRotationMatrix3x3 MATRIX_TO_COPY(EXPECTED_VALUE);

    // [Execution]
    QRotationMatrix3x3 matrixUT = MATRIX_TO_COPY;

    // [Verification]
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE_FOR_00);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE_FOR_01);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE_FOR_02);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE_FOR_10);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE_FOR_11);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE_FOR_12);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE_FOR_20);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE_FOR_21);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE_FOR_22);
}

/// <summary>
/// Checks that every matrix element is copied to the right destination matrix element.
/// </summary>
QTEST_CASE ( Constructor3_EveryElementCopiedToCorrespondingElement_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_1, SQFloat::_2, SQFloat::_3,
                                        SQFloat::_4, SQFloat::_5, SQFloat::_6,
                                        SQFloat::_7, SQFloat::_8, SQFloat::_9);

    // [Execution]
    QRotationMatrix3x3 matrixUT(EXPECTED_VALUE);

    // [Verification]
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that the expected matrix is obtained when using common angles.
/// </summary>
QTEST_CASE ( Constructor4_ExpectedValueIsObtainedWhenUsingCommonAngles_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QBaseMatrix3x3 EXPECTED_VALUE((float_q)0.61237243569579447,  (float_q)0.70710678118654757,     (float_q)0.35355339059327373,
                                        (float_q)-0.5,                 (float_q)4.3297802811774670e-017, (float_q)0.86602540378443860,
                                        (float_q)0.61237243569579458,  (float_q)-0.70710678118654746,    (float_q)0.35355339059327384);

    // Reference values obtained from following DirectX SDK statement:
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationYawPitchRoll(&rotm, SQAngle::_ThirdPi, SQAngle::_QuarterPi, SQAngle::_HalfPi);

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ROTATION_X = SQAngle::_45;
        const float_q ROTATION_Y = SQAngle::_60;
        const float_q ROTATION_Z = SQAngle::_90;
    #else
        const float_q ROTATION_X = SQAngle::_QuarterPi;
        const float_q ROTATION_Y = SQAngle::_ThirdPi;
        const float_q ROTATION_Z = SQAngle::_HalfPi;
    #endif

    // [Execution]
    QRotationMatrix3x3 matrixUT(ROTATION_X, ROTATION_Y, ROTATION_Z);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that a neutral rotation matrix (an identity matrix) is obtained when angles equal zero.
/// </summary>
QTEST_CASE ( Constructor4_NeutralRotationIsObtainedWhenAnglesEqualZero_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QBaseMatrix3x3 NEUTRAL_ROTATION = QRotationMatrix3x3::GetIdentity();

    const float_q ANGLES = SQFloat::_0;

    // [Execution]
    QRotationMatrix3x3 matrixUT(ANGLES, ANGLES, ANGLES);

    // [Verification]
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], NEUTRAL_ROTATION.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], NEUTRAL_ROTATION.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], NEUTRAL_ROTATION.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], NEUTRAL_ROTATION.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], NEUTRAL_ROTATION.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], NEUTRAL_ROTATION.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], NEUTRAL_ROTATION.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], NEUTRAL_ROTATION.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], NEUTRAL_ROTATION.ij[2][2]);
}

/// <summary>
/// Checks that the rotation follows left-handed rules.
/// </summary>
QTEST_CASE ( Constructor4_FollowsLeftHandedRules_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ROTATION_ANGLE = SQAngle::_90;
    #else
        const float_q ROTATION_ANGLE = SQAngle::_HalfPi;
    #endif

    const QRotationMatrix3x3 ROTATION_AROUND_X(ROTATION_ANGLE, SQFloat::_0, SQFloat::_0);
    const QRotationMatrix3x3 ROTATION_AROUND_Y(SQFloat::_0, ROTATION_ANGLE, SQFloat::_0);
    const QRotationMatrix3x3 ROTATION_AROUND_Z(SQFloat::_0, SQFloat::_0, ROTATION_ANGLE);

    const QVector3 EXPECTED_POSITION_AFTER_ROTATION_AROUND_X(SQFloat::_0, SQFloat::_0, SQFloat::_1);
    const QVector3 EXPECTED_POSITION_AFTER_ROTATION_AROUND_Y(SQFloat::_0, SQFloat::_0, SQFloat::_1);
    const QVector3 EXPECTED_POSITION_AFTER_ROTATION_AROUND_Z(-SQFloat::_1, SQFloat::_0, SQFloat::_0);

    const QVector3 POSITION_BEFORE_ROTATION_AROUND_X(SQFloat::_0, SQFloat::_1, SQFloat::_0);
    const QVector3 POSITION_BEFORE_ROTATION_AROUND_Y(-SQFloat::_1, SQFloat::_0, SQFloat::_0);
    const QVector3 POSITION_BEFORE_ROTATION_AROUND_Z(SQFloat::_0, SQFloat::_1, SQFloat::_0);

    // [Execution]
    QVector3 vPoint1UT = POSITION_BEFORE_ROTATION_AROUND_X.Transform(ROTATION_AROUND_X);
    QVector3 vPoint2UT = POSITION_BEFORE_ROTATION_AROUND_Y.Transform(ROTATION_AROUND_Y);
    QVector3 vPoint3UT = POSITION_BEFORE_ROTATION_AROUND_Z.Transform(ROTATION_AROUND_Z);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(vPoint1UT.x, EXPECTED_POSITION_AFTER_ROTATION_AROUND_X.x) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint1UT.y, EXPECTED_POSITION_AFTER_ROTATION_AROUND_X.y) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint1UT.z, EXPECTED_POSITION_AFTER_ROTATION_AROUND_X.z) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint2UT.x, EXPECTED_POSITION_AFTER_ROTATION_AROUND_Y.x) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint2UT.y, EXPECTED_POSITION_AFTER_ROTATION_AROUND_Y.y) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint2UT.z, EXPECTED_POSITION_AFTER_ROTATION_AROUND_Y.z) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint3UT.x, EXPECTED_POSITION_AFTER_ROTATION_AROUND_Z.x) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint3UT.y, EXPECTED_POSITION_AFTER_ROTATION_AROUND_Z.y) );
    BOOST_CHECK( SQFloat::AreEqual(vPoint3UT.z, EXPECTED_POSITION_AFTER_ROTATION_AROUND_Z.z) );
}

/// <summary>
/// Checks that the expected matrix is obtained when using a common normalized axis and angle.
/// </summary>
QTEST_CASE ( Constructor5_ExpectedValueIsObtainedWhenUsingCommonNormalizedAxisAndAngle_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;

    const QBaseMatrix3x3 EXPECTED_VALUE((float_q)0.72802772538750848,  (float_q)0.60878859791576267,   (float_q)-0.31520164040634452,
                                        (float_q)-0.52510482111191903, (float_q)0.79079055799039111,   (float_q)0.3145079017103789,
                                        (float_q)0.44072730561210988,  (float_q)-0.063456571298848269, (float_q)0.89539527899519555);

    // Reference values obtained from following DirectX SDK statement:
    // D3DXVECTOR3 vAxis(1, 2, 3);
    // D3DXVec3Normalize(&vAxis, &vAxis);
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationAxis(&rotm, &vAxis, SQAngle::_QuarterPi);

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ANGLE = SQAngle::_45;
    #else
        const float_q ANGLE = SQAngle::_QuarterPi;
    #endif

    const QVector3 AXIS = QVector3(SQFloat::_1, SQFloat::_2, SQFloat::_3).Normalize();

    // [Execution]
    QRotationMatrix3x3 matrixUT(ANGLE, AXIS);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that the elements of the matrix's diagonal equal the cosine of the rotation angle when a null vector is used as axis.
/// </summary>
QTEST_CASE ( Constructor5_MatrixDiagonalElementsEqualCosineOfAngleWhenUsingNullVectorAsAxisAndNonZeroAngle_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;

    // Reference values obtained from following DirectX SDK statement:
    // D3DXVECTOR3 vAxis(0, 0, 0);
    // D3DXVec3Normalize(&vAxis, &vAxis);
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationAxis(&rotm, &vAxis, SQAngle::_ThirdPi);

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ANGLE = SQAngle::_60;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q ANGLE = SQAngle::_ThirdPi;
    #endif

    const QVector3 AXIS = QVector3(SQFloat::_0, SQFloat::_0, SQFloat::_0);

    const float_q COS_ANGLE = cos_q(SQAngle::_ThirdPi);

    const QBaseMatrix3x3 EXPECTED_VALUE(COS_ANGLE,   SQFloat::_0, SQFloat::_0,
                                        SQFloat::_0, COS_ANGLE,   SQFloat::_0,
                                        SQFloat::_0, SQFloat::_0, COS_ANGLE);

    // [Execution]
    QRotationMatrix3x3 matrixUT(ANGLE, AXIS);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that a neutral rotation matrix (an Identity matrix) is obtained when the angle equals zero and the axis is a normalized common vector.
/// </summary>
QTEST_CASE ( Constructor5_NeutralRotationIsObtainedWhenUsingNormalizedCommonVectorAndAngleEqualsZero_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;

    // Reference values obtained from following DirectX SDK statement:
    // D3DXVECTOR3 vAxis(1, 2, 3);
    // D3DXVec3Normalize(&vAxis, &vAxis);
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationAxis(&rotm, &vAxis, 0);
    const float_q ANGLE = SQFloat::_0;
    const QVector3 AXIS = QVector3(SQFloat::_1, SQFloat::_2, SQFloat::_3).Normalize();
    const QRotationMatrix3x3 EXPECTED_VALUE = QRotationMatrix3x3::GetIdentity();

    // [Execution]
    QRotationMatrix3x3 matrixUT(ANGLE, AXIS);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that a neutral rotation matrix (an Identity matrix) is obtained when the angle equals zero and the axis is a null vector.
/// </summary>
QTEST_CASE ( Constructor5_NeutralRotationIsObtainedWhenUsingNullVectorAndAngleEqualsZero_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;

    // Reference values obtained from following DirectX SDK statement:
    // D3DXVECTOR3 vAxis(0, 0, 0);
    // D3DXVec3Normalize(&vAxis, &vAxis);
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationAxis(&rotm, &vAxis, 0);
    const float_q ANGLE = SQFloat::_0;
    const QVector3 AXIS = QVector3::GetNullVector();
    const QRotationMatrix3x3 EXPECTED_VALUE = QRotationMatrix3x3::GetIdentity();

    // [Execution]
    QRotationMatrix3x3 matrixUT(ANGLE, AXIS);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that the obtained value is different when the input axis vector is normalized or not.
/// </summary>
QTEST_CASE ( Constructor5_ObtainedValueIsDifferentWhenAxisIsNormalizedOrNot_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ANGLE = SQAngle::_45;
    #else
        const float_q ANGLE = SQAngle::_QuarterPi;
    #endif

    const QVector3 NORMALIZED_AXIS = QVector3(SQFloat::_1, SQFloat::_2, SQFloat::_3).Normalize();
    const QVector3 NONNORMALIZED_AXIS = QVector3(SQFloat::_1, SQFloat::_2, SQFloat::_3);

    // [Execution]
    QRotationMatrix3x3 matrixWhenAxisIsNormalizedUT(ANGLE, NORMALIZED_AXIS);
    QRotationMatrix3x3 matrixWhenAxisIsNotNormalizedUT(ANGLE, NONNORMALIZED_AXIS);

    // [Verification]
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[0][0], matrixWhenAxisIsNotNormalizedUT.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[0][1], matrixWhenAxisIsNotNormalizedUT.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[0][2], matrixWhenAxisIsNotNormalizedUT.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[1][0], matrixWhenAxisIsNotNormalizedUT.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[1][1], matrixWhenAxisIsNotNormalizedUT.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[1][2], matrixWhenAxisIsNotNormalizedUT.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[2][0], matrixWhenAxisIsNotNormalizedUT.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[2][1], matrixWhenAxisIsNotNormalizedUT.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreNotEqual(matrixWhenAxisIsNormalizedUT.ij[2][2], matrixWhenAxisIsNotNormalizedUT.ij[2][2]) );
}

/// <summary>
/// Checks that the rotation follows left-handed rules.
/// </summary>
QTEST_CASE ( Constructor5_FollowsLeftHandedRules_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ROTATION_ANGLE = SQAngle::_90;
    #else
        const float_q ROTATION_ANGLE = SQAngle::_HalfPi;
    #endif

    const QVector3 AXIS = QVector3::GetUnitVectorX();
    const QRotationMatrix3x3 ROTATION(ROTATION_ANGLE, AXIS);

    const QVector3 POSITION_BEFORE_ROTATION(SQFloat::_0, SQFloat::_1, SQFloat::_0);
    const QVector3 EXPECTED_POSITION_AFTER_ROTATION(SQFloat::_0, SQFloat::_0, SQFloat::_1);

    // [Execution]
    QVector3 vPointUT = POSITION_BEFORE_ROTATION.Transform(ROTATION);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(vPointUT.x, EXPECTED_POSITION_AFTER_ROTATION.x) );
    BOOST_CHECK( SQFloat::AreEqual(vPointUT.y, EXPECTED_POSITION_AFTER_ROTATION.y) );
    BOOST_CHECK( SQFloat::AreEqual(vPointUT.z, EXPECTED_POSITION_AFTER_ROTATION.z) );
}

/// <summary>
/// Checks that the expected matrix is obtained when using a common normalized quaternion.
/// </summary>
QTEST_CASE ( Constructor6_ExpectedValueIsObtainedWhenUsingCommonNormalizedQuaternion_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QQuaternion;

    const QBaseMatrix3x3 EXPECTED_VALUE((float_q)0.13333333333333353,  (float_q)0.93333333333333324, (float_q)-0.33333333333333326,
                                        (float_q)-0.66666666666666663, (float_q)0.33333333333333348, (float_q)0.66666666666666652,
                                        (float_q)0.73333333333333317,  (float_q)0.13333333333333336, (float_q)0.66666666666666674);

    // Reference values obtained from following DirectX SDK statement:
    // D3DXQUATERNION Q1(1, 2, 3, 4);
    // D3DXQuaternionNormalize(&Q1, &Q1);
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationQuaternion(&rotm, &Q1);

    const QQuaternion QUATERNION = QQuaternion(SQFloat::_1, SQFloat::_2, SQFloat::_3, SQFloat::_4).Normalize();

    // [Execution]
    QRotationMatrix3x3 matrixUT(QUATERNION);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that the obtained value is a neutral rotation (an Identity matrix) when the input is a null quaternion.
/// </summary>
QTEST_CASE ( Constructor6_NeutralRotationIsObtainedWhenQuaternionIsNull_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QQuaternion;

    const QRotationMatrix3x3 EXPECTED_VALUE = QRotationMatrix3x3::GetIdentity();

    // Reference values obtained from following DirectX SDK statement:
    // D3DXQUATERNION Q1(0, 0, 0, 0);
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationQuaternion(&rotm, &Q1);

    const QQuaternion QUATERNION = QQuaternion(SQFloat::_0, SQFloat::_0, SQFloat::_0, SQFloat::_0);

    // [Execution]
    QRotationMatrix3x3 matrixUT(QUATERNION);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that the obtained value is a neutral rotation (an Identity matrix) when the input is an identity quaternion.
/// </summary>
QTEST_CASE ( Constructor6_NeutralRotationIsObtainedWhenQuaternionIsIdentity_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QQuaternion;

    const QRotationMatrix3x3 EXPECTED_VALUE = QRotationMatrix3x3::GetIdentity();

    // Reference values obtained from following DirectX SDK statements:
    // D3DXQUATERNION Q1(0, 0, 0, 1);
    // D3DXMATRIX rotm;
    // D3DXMatrixRotationQuaternion(&rotm, &Q1);

    const QQuaternion QUATERNION = QQuaternion::GetIdentity();

    // [Execution]
    QRotationMatrix3x3 matrixUT(QUATERNION);

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that all the elements of the returned matrix equal zero but those that are in the diagonal, which equal one.
/// </summary>
QTEST_CASE ( GetIdentity_AllElementsEqualZeroAndDiagonalEqualsOne_Test )
{
    // [Preparation]
    const float_q EXPECTED_VALUE_FOR_ALL = SQFloat::_0;
    const float_q EXPECTED_VALUE_FOR_DIAGONAL = SQFloat::_1;

	// [Execution]
    QRotationMatrix3x3 matrixUT = QRotationMatrix3x3::GetIdentity();

    // [Verification]
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE_FOR_DIAGONAL);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE_FOR_DIAGONAL);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE_FOR_DIAGONAL);
}

/// <summary>
/// Checks that two common matrices are correctly multiplied.
/// </summary>
QTEST_CASE ( OperatorProduct1_CommonMatricesAreCorrectlyMultiplied_Test )
{
    // [Preparation]
#if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
    const float_q ANGLE_X1 = SQFloat::_1;
    const float_q ANGLE_Y1 = SQFloat::_2;
    const float_q ANGLE_Z1 = SQFloat::_3;
    const float_q ANGLE_X2 = SQFloat::_4;
    const float_q ANGLE_Y2 = SQFloat::_5;
    const float_q ANGLE_Z2 = SQFloat::_6;
#elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
    const float_q ANGLE_X1 = SQAngle::RadiansToDegrees(SQFloat::_1);
    const float_q ANGLE_Y1 = SQAngle::RadiansToDegrees(SQFloat::_2);
    const float_q ANGLE_Z1 = SQAngle::RadiansToDegrees(SQFloat::_3);
    const float_q ANGLE_X2 = SQAngle::RadiansToDegrees(SQFloat::_4);
    const float_q ANGLE_Y2 = SQAngle::RadiansToDegrees(SQFloat::_5);
    const float_q ANGLE_Z2 = SQAngle::RadiansToDegrees(SQFloat::_6);
#endif

    const QRotationMatrix3x3 OPERAND1(ANGLE_X1, ANGLE_Y1, ANGLE_Z1);
    const QRotationMatrix3x3 OPERAND2(ANGLE_X2, ANGLE_Y2, ANGLE_Z2);

    // Reference values obtained from following DirectX SDK statements:
    // D3DXMATRIX rotm1;
    // D3DXMatrixRotationYawPitchRoll(&rotm1, 2, 1, 3);
    // D3DXMATRIX rotm2;
    // D3DXMatrixRotationYawPitchRoll(&rotm2, 5, 4, 6);
    // D3DXMATRIX rotm = rotm1 * rotm2;
    const float_q EXPECTED_VALUE_FOR_00 = (float_q)0.62862116970450643;
    const float_q EXPECTED_VALUE_FOR_01 = (float_q)0.69098397942738221;
    const float_q EXPECTED_VALUE_FOR_02 = (float_q)0.35689853624529905;
    const float_q EXPECTED_VALUE_FOR_10 = (float_q)-0.16601991351523826;
    const float_q EXPECTED_VALUE_FOR_11 = (float_q)0.56755877119703968;
    const float_q EXPECTED_VALUE_FOR_12 = (float_q)-0.80642075218442832;
    const float_q EXPECTED_VALUE_FOR_20 = (float_q)-0.75978471511062295;
    const float_q EXPECTED_VALUE_FOR_21 = (float_q)0.44768089239100362;
    const float_q EXPECTED_VALUE_FOR_22 = (float_q)0.47149655913088467;

    // [Execution]
    QRotationMatrix3x3 matrixUT = OPERAND1 * OPERAND2;

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE_FOR_00) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE_FOR_01) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE_FOR_02) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE_FOR_10) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE_FOR_11) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE_FOR_12) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE_FOR_20) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE_FOR_21) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE_FOR_22) );
}

/// <summary>
/// Checks that product operation is not commutative.
/// </summary>
QTEST_CASE ( OperatorProduct1_ProductIsNotCommutative_Test )
{
    // [Preparation]
    const QRotationMatrix3x3 OPERAND1(SQFloat::_1, SQFloat::_2, SQFloat::_3);
    const QRotationMatrix3x3 OPERAND2(SQFloat::_4, SQFloat::_5, SQFloat::_6);

    // [Execution]
    QRotationMatrix3x3 matrix1UT = OPERAND1 * OPERAND2;
    QRotationMatrix3x3 matrix2UT = OPERAND2 * OPERAND1;

    // [Verification]
    BOOST_CHECK( matrix1UT != matrix2UT );
}

/// <summary>
/// Checks that two common matrices are correctly multiplied.
/// </summary>
QTEST_CASE ( OperatorProduct2_CommonMatricesAreCorrectlyMultiplied_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QTransformationMatrix;
    using Kinesis::QuimeraEngine::Tools::Math::QScalingMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QMatrix4x4;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q ANGLE_X = SQFloat::_1;
        const float_q ANGLE_Y = SQFloat::_2;
        const float_q ANGLE_Z = SQFloat::_3;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
        const float_q ANGLE_X = SQAngle::RadiansToDegrees(SQFloat::_1);
        const float_q ANGLE_Y = SQAngle::RadiansToDegrees(SQFloat::_2);
        const float_q ANGLE_Z = SQAngle::RadiansToDegrees(SQFloat::_3);
    #endif

    const QRotationMatrix3x3 ROTATION(ANGLE_X, ANGLE_Y, ANGLE_Z);
    const QScalingMatrix3x3 SCALE(SQFloat::_4, SQFloat::_5, SQFloat::_6);

    const QMatrix4x4 EXPECTED_VALUE((float_q)2.0798394123794113,  (float_q)0.38123732879438366, (float_q)5.1046851719628510,  SQFloat::_0,
                                    (float_q)-2.7950541643506361, (float_q)-2.6744761435268858, (float_q)2.8499469971167666,  SQFloat::_0,
                                    (float_q)1.9651819857355277,  (float_q)-4.2073549240394827, (float_q)-1.3490705721969174, SQFloat::_0,
                                    SQFloat::_0,                  SQFloat::_0,                  SQFloat::_0,                  SQFloat::_1);

    // [Execution]
    QTransformationMatrix<QMatrix4x4> matrixUT = ROTATION * SCALE;

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][3], EXPECTED_VALUE.ij[0][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][3], EXPECTED_VALUE.ij[1][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][3], EXPECTED_VALUE.ij[2][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][0], EXPECTED_VALUE.ij[3][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][1], EXPECTED_VALUE.ij[3][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][2], EXPECTED_VALUE.ij[3][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][3], EXPECTED_VALUE.ij[3][3]) );
}

/// <summary>
/// Checks that two common matrices are correctly multiplied.
/// </summary>
QTEST_CASE ( OperatorProduct3_CommonMatricesAreCorrectlyMultiplied_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QTransformationMatrix;
    using Kinesis::QuimeraEngine::Tools::Math::QTranslationMatrix;
    using Kinesis::QuimeraEngine::Tools::Math::QMatrix4x4;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q ANGLE_X = SQFloat::_1;
        const float_q ANGLE_Y = SQFloat::_2;
        const float_q ANGLE_Z = SQFloat::_3;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
        const float_q ANGLE_X = SQAngle::RadiansToDegrees(SQFloat::_1);
        const float_q ANGLE_Y = SQAngle::RadiansToDegrees(SQFloat::_2);
        const float_q ANGLE_Z = SQAngle::RadiansToDegrees(SQFloat::_3);
    #endif

    const QRotationMatrix3x3 ROTATION(ANGLE_X, ANGLE_Y, ANGLE_Z);
    const QTranslationMatrix<QMatrix4x4> TRANSLATION(SQFloat::_4, SQFloat::_5, SQFloat::_6);

    const QMatrix4x4 EXPECTED_VALUE((float_q)0.51995985309485282,  (float_q)0.076247465758876726, (float_q)0.85078086199380853,  SQFloat::_0,
                                    (float_q)-0.69876354108765903, (float_q)-0.53489522870537720, (float_q)0.47499116618612774,  SQFloat::_0,
                                    (float_q)0.49129549643388193,  (float_q)-0.84147098480789650, (float_q)-0.22484509536615291, SQFloat::_0,
                                    SQFloat::_4,                   SQFloat::_5,                   SQFloat::_6,                   SQFloat::_1);

    // [Execution]
    QTransformationMatrix<QMatrix4x4> matrixUT = ROTATION * TRANSLATION;

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][3], EXPECTED_VALUE.ij[0][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][3], EXPECTED_VALUE.ij[1][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][3], EXPECTED_VALUE.ij[2][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][0], EXPECTED_VALUE.ij[3][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][1], EXPECTED_VALUE.ij[3][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][2], EXPECTED_VALUE.ij[3][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][3], EXPECTED_VALUE.ij[3][3]) );
}

/// <summary>
/// Checks that two common matrices are correctly multiplied.
/// </summary>
QTEST_CASE ( OperatorProduct4_CommonMatricesAreCorrectlyMultiplied_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QTransformationMatrix;
    using Kinesis::QuimeraEngine::Tools::Math::QTranslationMatrix;
    using Kinesis::QuimeraEngine::Tools::Math::QMatrix4x3;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q ANGLE_X = SQFloat::_1;
        const float_q ANGLE_Y = SQFloat::_2;
        const float_q ANGLE_Z = SQFloat::_3;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
        const float_q ANGLE_X = SQAngle::RadiansToDegrees(SQFloat::_1);
        const float_q ANGLE_Y = SQAngle::RadiansToDegrees(SQFloat::_2);
        const float_q ANGLE_Z = SQAngle::RadiansToDegrees(SQFloat::_3);
    #endif

    const QRotationMatrix3x3 ROTATION(ANGLE_X, ANGLE_Y, ANGLE_Z);
    const QTranslationMatrix<QMatrix4x3> TRANSLATION(SQFloat::_4, SQFloat::_5, SQFloat::_6);

    const QMatrix4x3 EXPECTED_VALUE((float_q)0.51995985309485282,  (float_q)0.076247465758876726, (float_q)0.85078086199380853,
                                    (float_q)-0.69876354108765903, (float_q)-0.53489522870537720, (float_q)0.47499116618612774,
                                    (float_q)0.49129549643388193,  (float_q)-0.84147098480789650, (float_q)-0.22484509536615291,
                                    SQFloat::_4,                   SQFloat::_5,                   SQFloat::_6);

    // [Execution]
    QTransformationMatrix<QMatrix4x3> matrixUT = ROTATION * TRANSLATION;

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][0], EXPECTED_VALUE.ij[3][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][1], EXPECTED_VALUE.ij[3][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][2], EXPECTED_VALUE.ij[3][2]) );
}

/// <summary>
/// Checks that two common matrices are correctly multiplied.
/// </summary>
QTEST_CASE ( OperatorProduct5_CommonMatricesAreCorrectlyMultiplied_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QTransformationMatrix;
    using Kinesis::QuimeraEngine::Tools::Math::QMatrix4x4;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q ANGLE_X = SQFloat::_1;
        const float_q ANGLE_Y = SQFloat::_2;
        const float_q ANGLE_Z = SQFloat::_3;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
        const float_q ANGLE_X = SQAngle::RadiansToDegrees(SQFloat::_1);
        const float_q ANGLE_Y = SQAngle::RadiansToDegrees(SQFloat::_2);
        const float_q ANGLE_Z = SQAngle::RadiansToDegrees(SQFloat::_3);
    #endif

    const QRotationMatrix3x3 ROTATION(ANGLE_X, ANGLE_Y, ANGLE_Z);
    const QTransformationMatrix<QMatrix4x4> TRANSFORMATION(QMatrix4x4(SQFloat::_4,    SQFloat::_5,    SQFloat::_6,    SQFloat::_7,
                                                                      SQFloat::_8,    SQFloat::_9,    SQFloat::_10,   (float_q)11.0,
                                                                      (float_q)12.0, (float_q)13.0, (float_q)14.0, (float_q)15.0,
                                                                      (float_q)16.0, (float_q)17.0, (float_q)18.0, (float_q)19.0));

    const QMatrix4x4 EXPECTED_VALUE((float_q)12.899189482376126,  (float_q)14.346177663223664,  (float_q)15.793165844071204,  SQFloat::_0,
                                    (float_q)-1.374321999760121,  (float_q)-2.1329896033670286, (float_q)-2.891657206973937,  SQFloat::_0,
                                    (float_q)-7.4647270371214791, (float_q)-8.0397476208616467, (float_q)-8.6147682046018144, SQFloat::_0,
                                    (float_q)16.0,                (float_q)17.0,                (float_q)18.0,                SQFloat::_1);

    // [Execution]
    QTransformationMatrix<QMatrix4x4> matrixUT = ROTATION * TRANSFORMATION;

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][3], EXPECTED_VALUE.ij[0][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][3], EXPECTED_VALUE.ij[1][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][3], EXPECTED_VALUE.ij[2][3]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][0], EXPECTED_VALUE.ij[3][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][1], EXPECTED_VALUE.ij[3][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][2], EXPECTED_VALUE.ij[3][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][3], EXPECTED_VALUE.ij[3][3]) );
}

/// <summary>
/// Checks that two common matrices are correctly multiplied.
/// </summary>
QTEST_CASE ( OperatorProduct6_CommonMatricesAreCorrectlyMultiplied_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QTransformationMatrix;
    using Kinesis::QuimeraEngine::Tools::Math::QMatrix4x3;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q ANGLE_X = SQFloat::_1;
        const float_q ANGLE_Y = SQFloat::_2;
        const float_q ANGLE_Z = SQFloat::_3;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
        const float_q ANGLE_X = SQAngle::RadiansToDegrees(SQFloat::_1);
        const float_q ANGLE_Y = SQAngle::RadiansToDegrees(SQFloat::_2);
        const float_q ANGLE_Z = SQAngle::RadiansToDegrees(SQFloat::_3);
    #endif

    const QRotationMatrix3x3 ROTATION(ANGLE_X, ANGLE_Y, ANGLE_Z);
    const QTransformationMatrix<QMatrix4x3> TRANSFORMATION(QMatrix4x3(SQFloat::_4,    SQFloat::_5,    SQFloat::_6,
                                                                      SQFloat::_8,    SQFloat::_9,    SQFloat::_10,
                                                                      (float_q)12.0, (float_q)13.0, (float_q)14.0,
                                                                      (float_q)16.0, (float_q)17.0, (float_q)18.0));

    const QMatrix4x3 EXPECTED_VALUE((float_q)12.899189482376126,  (float_q)14.346177663223664,  (float_q)15.793165844071204,
                                    (float_q)-1.374321999760121,  (float_q)-2.1329896033670286, (float_q)-2.8916572069739370,
                                    (float_q)-7.4647270371214791, (float_q)-8.0397476208616467, (float_q)-8.6147682046018144,
                                    (float_q)16.0,                (float_q)17.0,                (float_q)18.0);

    // [Execution]
    QTransformationMatrix<QMatrix4x3> matrixUT = ROTATION * TRANSFORMATION;

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][0], EXPECTED_VALUE.ij[3][0]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][1], EXPECTED_VALUE.ij[3][1]) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[3][2], EXPECTED_VALUE.ij[3][2]) );
}

/// <summary>
/// Checks that every matrix element is assigned to the right destination matrix element.
/// </summary>
QTEST_CASE ( OperatorAssignation_EveryElementAssignedToCorrespondingElement_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_1, SQFloat::_2, SQFloat::_3,
                                        SQFloat::_4, SQFloat::_5, SQFloat::_6,
                                        SQFloat::_7, SQFloat::_8, SQFloat::_9);

    // [Execution]
    QRotationMatrix3x3 matrixUT;
    matrixUT = EXPECTED_VALUE;

    // [Verification]
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that two common matrices are correctly multiplied and assigned.
/// </summary>
QTEST_CASE ( OperatorProductAssignation_CommonMatricesAreCorrectlyMultipliedAndAssigned_Test )
{
    // [Preparation]
    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q ANGLE_X1 = SQFloat::_1;
        const float_q ANGLE_Y1 = SQFloat::_2;
        const float_q ANGLE_Z1 = SQFloat::_3;
        const float_q ANGLE_X2 = SQFloat::_4;
        const float_q ANGLE_Y2 = SQFloat::_5;
        const float_q ANGLE_Z2 = SQFloat::_6;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        using Kinesis::QuimeraEngine::Tools::Math::SQAngle;
        const float_q ANGLE_X1 = SQAngle::RadiansToDegrees(SQFloat::_1);
        const float_q ANGLE_Y1 = SQAngle::RadiansToDegrees(SQFloat::_2);
        const float_q ANGLE_Z1 = SQAngle::RadiansToDegrees(SQFloat::_3);
        const float_q ANGLE_X2 = SQAngle::RadiansToDegrees(SQFloat::_4);
        const float_q ANGLE_Y2 = SQAngle::RadiansToDegrees(SQFloat::_5);
        const float_q ANGLE_Z2 = SQAngle::RadiansToDegrees(SQFloat::_6);
    #endif

    const QRotationMatrix3x3 OPERAND1(ANGLE_X1, ANGLE_Y1, ANGLE_Z1);
    const QRotationMatrix3x3 OPERAND2(ANGLE_X2, ANGLE_Y2, ANGLE_Z2);

    // Reference values obtained from following DirectX SDK statements:
    // D3DXMATRIX rotm1;
    // D3DXMatrixRotationYawPitchRoll(&rotm1, 2, 1, 3);
    // D3DXMATRIX rotm2;
    // D3DXMatrixRotationYawPitchRoll(&rotm2, 5, 4, 6);
    // D3DXMATRIX rotm = rotm1 * rotm2;
    const float_q EXPECTED_VALUE_FOR_00 = (float_q)0.62862116970450643;
    const float_q EXPECTED_VALUE_FOR_01 = (float_q)0.69098397942738221;
    const float_q EXPECTED_VALUE_FOR_02 = (float_q)0.35689853624529905;
    const float_q EXPECTED_VALUE_FOR_10 = (float_q)-0.16601991351523826;
    const float_q EXPECTED_VALUE_FOR_11 = (float_q)0.56755877119703968;
    const float_q EXPECTED_VALUE_FOR_12 = (float_q)-0.80642075218442832;
    const float_q EXPECTED_VALUE_FOR_20 = (float_q)-0.75978471511062295;
    const float_q EXPECTED_VALUE_FOR_21 = (float_q)0.44768089239100362;
    const float_q EXPECTED_VALUE_FOR_22 = (float_q)0.47149655913088467;

    // [Execution]
    QRotationMatrix3x3 matrixUT = OPERAND1;
    matrixUT *= OPERAND2;

    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][0], EXPECTED_VALUE_FOR_00) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][1], EXPECTED_VALUE_FOR_01) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[0][2], EXPECTED_VALUE_FOR_02) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][0], EXPECTED_VALUE_FOR_10) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][1], EXPECTED_VALUE_FOR_11) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[1][2], EXPECTED_VALUE_FOR_12) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][0], EXPECTED_VALUE_FOR_20) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][1], EXPECTED_VALUE_FOR_21) );
    BOOST_CHECK( SQFloat::AreEqual(matrixUT.ij[2][2], EXPECTED_VALUE_FOR_22) );
}

/// <summary>
/// Checks that product operation is not commutative.
/// </summary>
QTEST_CASE ( OperatorProductAssignation_ProductIsNotCommutative_Test )
{
    // [Preparation]
    const QRotationMatrix3x3 OPERAND1(SQFloat::_1, SQFloat::_2, SQFloat::_3);
    const QRotationMatrix3x3 OPERAND2(SQFloat::_4, SQFloat::_5, SQFloat::_6);

    // [Execution]
    QRotationMatrix3x3 matrix1UT = OPERAND1;
    matrix1UT *= OPERAND2;
    QRotationMatrix3x3 matrix2UT = OPERAND2;
    matrix2UT *= OPERAND1;

    // [Verification]
    BOOST_CHECK( matrix1UT != matrix2UT );
}

/// <summary>
/// Checks if a matrix object is correctly multiplied and assigned when using itself (the object) as the other operand.
/// </summary>
QTEST_CASE ( OperatorProductAssignation_MatrixObjectIsCorrectlyMultipliedAndAssignedToItself_Test )
{
    // [Preparation]
    const QRotationMatrix3x3 OPERAND(SQFloat::_1, SQFloat::_2, SQFloat::_3);

    QRotationMatrix3x3 EXPECTED_VALUE = OPERAND;
    EXPECTED_VALUE *= OPERAND;

	// [Execution]
    QRotationMatrix3x3 matrixUT = OPERAND;
    matrixUT *= matrixUT;

    // [Verification]
    BOOST_CHECK(matrixUT == EXPECTED_VALUE);
}

/// <summary>
/// Checks that the matrix is correctly inverted.
/// </summary>
QTEST_CASE ( Invert_MatrixIsCorrectlyInverted_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QRotationMatrix3x3 ORIGINAL_VALUE(QBaseMatrix3x3(SQFloat::_1, SQFloat::_2, SQFloat::_3,
                                                           SQFloat::_4, SQFloat::_5, SQFloat::_6,
                                                           SQFloat::_7, SQFloat::_8, SQFloat::_9));

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_1, SQFloat::_4, SQFloat::_7,
                                        SQFloat::_2, SQFloat::_5, SQFloat::_8,
                                        SQFloat::_3, SQFloat::_6, SQFloat::_9);

    // [Execution]
    QRotationMatrix3x3 matrixUT = ORIGINAL_VALUE.Invert();

    // [Verification]
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that obtained angles equal expected values.
/// </summary>
QTEST_CASE ( GetRotation1_AnglesAreCorrectlyExtracted_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ROTATION_X = SQAngle::_45;
        const float_q EXPECTED_ROTATION_Y = SQAngle::_60;
        const float_q EXPECTED_ROTATION_Z = SQAngle::_90;
    #else
        const float_q EXPECTED_ROTATION_X = SQAngle::_QuarterPi;
        const float_q EXPECTED_ROTATION_Y = SQAngle::_ThirdPi;
        const float_q EXPECTED_ROTATION_Z = SQAngle::_HalfPi;
    #endif

    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3((float_q)0.61237243569579447, (float_q)0.70710678118654757,     (float_q)0.35355339059327373,
                                                     (float_q)-0.5,                (float_q)4.3297802811774670e-017, (float_q)0.86602540378443860,
                                                     (float_q)0.61237243569579458, (float_q)-0.70710678118654746,    (float_q)0.35355339059327384));

    // [Execution]
    float_q fRotationXUT = SQFloat::_0;
    float_q fRotationYUT = SQFloat::_0;
    float_q fRotationZUT = SQFloat::_0;
    
    ROTATION.GetRotation(fRotationXUT, fRotationYUT, fRotationZUT);
    
    // [Verification]
    BOOST_CHECK( SQFloat::AreEqual(fRotationXUT, EXPECTED_ROTATION_X, SMALLER_TOLERANCE) );
    BOOST_CHECK( SQFloat::AreEqual(fRotationYUT, EXPECTED_ROTATION_Y) );
    BOOST_CHECK( SQFloat::AreEqual(fRotationZUT, EXPECTED_ROTATION_Z) );
}

/// <summary>
/// Checks that obtained angles don't equal expected values due to gimbal lock limitation.
/// </summary>
QTEST_CASE ( GetRotation1_AnglesAreNotWhatExpectedWhenGimbalLockOccurs_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ROTATION_X = SQAngle::_90;
        const float_q EXPECTED_ROTATION_Y = SQAngle::_90;
        const float_q EXPECTED_ROTATION_Z = SQAngle::_60;
    #else
        const float_q EXPECTED_ROTATION_X = SQAngle::_HalfPi;
        const float_q EXPECTED_ROTATION_Y = SQAngle::_HalfPi;
        const float_q EXPECTED_ROTATION_Z = SQAngle::_ThirdPi;
    #endif

    // By rotating 90� (or PI/2) two gimbals, they become alligned so rotating any of them results in the same transformation
    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3((float_q)0.86602545,      (float_q)-3.7855173e-008, (float_q)-0.5,
                                                     (float_q)0.5,             (float_q)-2.1855692e-008, (float_q)0.86602545,
                                                     (float_q)-4.3711388e-008, (float_q)-1.0,            (float_q)1.9106855e-015));

    // [Execution]
    float_q fRotationXUT = SQFloat::_0;
    float_q fRotationYUT = SQFloat::_0;
    float_q fRotationZUT = SQFloat::_0;

    ROTATION.GetRotation(fRotationXUT, fRotationYUT, fRotationZUT);

    // [Verification]
    BOOST_CHECK( SQFloat::AreNotEqual(fRotationXUT, EXPECTED_ROTATION_X) ||
                 SQFloat::AreNotEqual(fRotationYUT, EXPECTED_ROTATION_Y) ||
                 SQFloat::AreNotEqual(fRotationZUT, EXPECTED_ROTATION_Z) );
}

/// <summary>
/// Checks that the obtained quaternion matches what expected.
/// </summary>
QTEST_CASE ( GetRotation2_MatrixIsCorrectlyConvertedInQuaternion_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QQuaternion;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ROTATION_X = SQAngle::_45;
        const float_q ROTATION_Y = SQAngle::_60;
        const float_q ROTATION_Z = SQAngle::_90;
    #else
        const float_q ROTATION_X = SQAngle::_QuarterPi;
        const float_q ROTATION_Y = SQAngle::_ThirdPi;
        const float_q ROTATION_Z = SQAngle::_HalfPi;
    #endif

    const QQuaternion EXPECTED_VALUE(ROTATION_X, ROTATION_Y, ROTATION_Z);

    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3((float_q)0.61237243569579447, (float_q)0.70710678118654757,     (float_q)0.35355339059327373,
                                                     (float_q)-0.5,                (float_q)4.3297802811774670e-017, (float_q)0.86602540378443860,
                                                     (float_q)0.61237243569579458, (float_q)-0.70710678118654746,    (float_q)0.35355339059327384));

    // [Execution]
    QQuaternion qResultUT;
    ROTATION.GetRotation(qResultUT);

    // [Verification]
    BOOST_CHECK( qResultUT == EXPECTED_VALUE );
}

/// <summary>
/// Checks that the obtained axis and angle match what expected.
/// </summary>
QTEST_CASE ( GetRotation3_MatrixIsCorrectlyConvertedInAxisAndAngle_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ANGLE = SQAngle::_60;
    #else
        const float_q EXPECTED_ANGLE = SQAngle::_ThirdPi;
    #endif

    const QVector3 EXPECTED_AXIS = QVector3(SQFloat::_1, SQFloat::_2, SQFloat::_3).Normalize();

    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3((float_q)0.53571428571428581,  (float_q)0.76579364625798496,   (float_q)-0.35576719274341861,
                                                     (float_q)-0.62293650340084217, (float_q)0.64285714285714302,   (float_q)0.4457407392288521,
                                                     (float_q)0.5700529070291328,   (float_q)-0.017169310657423609, (float_q)0.8214285714285714));

    // [Execution]
    QVector3 vAxisUT;
    float_q fAngleUT = SQFloat::_0;
    ROTATION.GetRotation(fAngleUT, vAxisUT);

    // [Verification]
    BOOST_CHECK( vAxisUT == EXPECTED_AXIS );
    BOOST_CHECK( SQFloat::AreEqual(fAngleUT, EXPECTED_ANGLE) );
}

/// <summary>
/// Checks that the rotation axis obtained is the inverse of which results from the opposite rotation.
/// </summary>
QTEST_CASE ( GetRotation3_ResultantAxisIsTheOppositeWhenRotationIsTheOpposite_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;

    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3((float_q)0.53571427,  (float_q)0.76579368,   (float_q)-0.35576719,
                                                     (float_q)-0.62293649, (float_q)0.64285713,   (float_q)0.44574076,
                                                     (float_q)0.57005292,  (float_q)-0.017169312, (float_q)0.82142854));
    const QRotationMatrix3x3 OPPOSITE_ROTATION = ROTATION.Invert();

    // [Execution]
    QVector3 vAxisUT;
    float_q fAngleUT = SQFloat::_0;
    ROTATION.GetRotation(fAngleUT, vAxisUT);

    QVector3 vAxisFromOppositeRotationUT;
    float_q fAngleFromOppositeRotationUT = SQFloat::_0;
    OPPOSITE_ROTATION.GetRotation(fAngleFromOppositeRotationUT, vAxisFromOppositeRotationUT);

    // [Verification]
    BOOST_CHECK( vAxisUT == -vAxisFromOppositeRotationUT );
    BOOST_CHECK( SQFloat::AreEqual(fAngleUT, fAngleFromOppositeRotationUT) );
}

/// <summary>
/// Checks that the obtained axis and angle are calculated correctly when rotation angle equals Pi (or 180�).
/// </summary>
QTEST_CASE ( GetRotation3_AxisAndAngleAreCalculatedNormallyWhenRotationAngleEqualsPiRadiansOr180Degrees_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ANGLE = SQAngle::_180;
    #elif QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_RADIANS
        const float_q EXPECTED_ANGLE = (float_q)3.1415926386886319;
    #endif

    const QVector3 EXPECTED_AXIS = QVector3(SQFloat::_1, SQFloat::_2, SQFloat::_3).Normalize();
    
    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3((float_q)-0.8571428571428571, (float_q)0.28571428571428586,  (float_q)0.42857142857142855,
                                                     (float_q)0.28571428571428564, (float_q)-0.42857142857142849, (float_q)0.85714285714285721,
                                                     (float_q)0.42857142857142866, (float_q)0.85714285714285721,  (float_q)0.28571428571428581));

    // [Execution]
    QVector3 vAxisUT;
    float_q fAngleUT = SQFloat::_0;
    ROTATION.GetRotation(fAngleUT, vAxisUT);

    // [Verification]
    BOOST_CHECK( vAxisUT == EXPECTED_AXIS );
    BOOST_CHECK( SQFloat::AreEqual(fAngleUT, EXPECTED_ANGLE) );
}

/// <summary>
/// Checks that the obtained axis is a null vector when the rotation angle is zero.
/// </summary>
QTEST_CASE ( GetRotation3_NullVectorIsReturnedWhenRotationAngleEqualsZero_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const float_q EXPECTED_ANGLE = SQFloat::_0;
    const QVector3 EXPECTED_AXIS = QVector3::GetNullVector();

    const QRotationMatrix3x3 ROTATION = QRotationMatrix3x3::GetIdentity(); // Identity is obtained when using zero as angle, it has nothing to do with the value of the axis

    // [Execution]
    QVector3 vAxisUT;
    float_q fAngleUT = SQFloat::_0;
    ROTATION.GetRotation(fAngleUT, vAxisUT);

    // [Verification]
    BOOST_CHECK( vAxisUT == EXPECTED_AXIS );
    BOOST_CHECK( SQFloat::AreEqual(fAngleUT, EXPECTED_ANGLE) );
}

/// <summary>
/// Checks that the obtained axis is normalized.
/// </summary>
QTEST_CASE ( GetRotation3_ResultantAxisIsNormalized_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector3;

    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3((float_q)0.53571428571428581,  (float_q)0.76579364625798496,   (float_q)-0.35576719274341861,
                                                     (float_q)-0.62293650340084217, (float_q)0.64285714285714302,   (float_q)0.4457407392288521,
                                                     (float_q)0.5700529070291328,   (float_q)-0.017169310657423609, (float_q)0.8214285714285714));

    // [Execution]
    QVector3 vAxisUT;
    float_q fAngleAux = SQFloat::_0;
    QVector3 vNormalizedAxisUT;
    ROTATION.GetRotation(fAngleAux, vAxisUT);

    vNormalizedAxisUT = vAxisUT.Normalize();

    // [Verification]
    BOOST_CHECK( vAxisUT == vNormalizedAxisUT );
}

/// <summary>
/// Checks that always returns 1.
/// </summary>
QTEST_CASE ( GetDeterminant_AlwaysReturnsOne_Test )
{
    // [Preparation]
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QRotationMatrix3x3 ROTATION(QBaseMatrix3x3(SQFloat::_1, SQFloat::_2, SQFloat::_3,
                                                     SQFloat::_4, SQFloat::_5, SQFloat::_6,
                                                     SQFloat::_7, SQFloat::_8, SQFloat::_9));
    const float_q EXPECTED_VALUE = SQFloat::_1;

    // [Execution]
    float_q fResultUT = ROTATION.GetDeterminant();

    // [Verification]
    BOOST_CHECK_EQUAL(fResultUT, EXPECTED_VALUE);
}

// End - Test Suite: QRotationMatrix3x3
QTEST_SUITE_END()
