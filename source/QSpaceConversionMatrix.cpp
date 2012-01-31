// [TERMS&CONDITIONS]

#include "QSpaceConversionMatrix.h"

#include "QBaseQuaternion.h"
#include "QVector3.h"
#include "QTranslationMatrix.h"
#include "QRotationMatrix3x3.h"
#include "QScaleMatrix3x3.h"
#include "QTransformationMatrix.h"

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
//##################		    |		    METHODS			 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

// Binary operators

QSpaceConversionMatrix QSpaceConversionMatrix::operator*(const QSpaceConversionMatrix &matrix) const
{
    QSpaceConversionMatrix aux;

    aux = ( this->As<const QMatrix4x4>() * matrix.As<const QMatrix4x4>() ).As<QSpaceConversionMatrix>();
    return aux;
}

QSpaceConversionMatrix& QSpaceConversionMatrix::operator*=(const QSpaceConversionMatrix &matrix)
{
    this->As<QMatrix4x4>() *= matrix.As<const QMatrix4x4>();
    return *this;
}

void QSpaceConversionMatrix::SetWorldSpaceMatrix(const QBaseVector3 &vTranslation, const QBaseQuaternion &qRotation, const QBaseVector3 &vScale)
{
    QTransformationMatrix<QMatrix4x4> aux(vTranslation, qRotation, vScale);

    *this = aux.As<QSpaceConversionMatrix>();
}

void QSpaceConversionMatrix::SetWorldSpaceMatrix(const QBaseVector4 &vTranslation, const QBaseQuaternion &qRotation, const QBaseVector3 &vScale)
{
    QTransformationMatrix<QMatrix4x4> aux(vTranslation, qRotation, vScale);

    *this = aux.As<QSpaceConversionMatrix>();
}

void QSpaceConversionMatrix::SetWorldSpaceMatrix(const QTransformationMatrix<QMatrix4x4> &translation, const QTransformationMatrix<QMatrix4x4> &rotation,
                                                 const QTransformationMatrix<QMatrix4x4> &scale)
{
    this->ij[0][0] = scale.ij[0][0] * rotation.ij[0][0];
    this->ij[0][1] = scale.ij[0][0] * rotation.ij[0][1];
    this->ij[0][2] = scale.ij[0][0] * rotation.ij[0][2];
    this->ij[0][3] = SQFloat::_0;

    this->ij[1][0] = scale.ij[1][1] * rotation.ij[1][0];
    this->ij[1][1] = scale.ij[1][1] * rotation.ij[1][1];
    this->ij[1][2] = scale.ij[1][1] * rotation.ij[1][2];
    this->ij[1][3] = SQFloat::_0;

    this->ij[2][0] = scale.ij[2][2] * rotation.ij[2][0];
    this->ij[2][1] = scale.ij[2][2] * rotation.ij[2][1];
    this->ij[2][2] = scale.ij[2][2] * rotation.ij[2][2];
    this->ij[2][3] = SQFloat::_0;

    this->ij[3][0] = translation.ij[3][0];
    this->ij[3][1] = translation.ij[3][1];
    this->ij[3][2] = translation.ij[3][2];
    this->ij[3][3] = SQFloat::_1;
}

void QSpaceConversionMatrix::SetViewSpaceMatrix(const QBaseVector3 &vPointOfView, const QBaseVector3 &vTarget,
                                                const QBaseVector3 &vUpDirection)
{
    QVector3 vZAxis(vTarget.x - vPointOfView.x, vTarget.y - vPointOfView.y, vTarget.z - vPointOfView.z);
    vZAxis.Normalize();

    QVector3 vXAxis(vUpDirection);
    vXAxis.CrossProduct(vZAxis);
    vXAxis.Normalize();

    QVector3 vYAxis(vZAxis);
    vYAxis.CrossProduct(vXAxis);

    this->ij[0][0] = vXAxis.x;
    this->ij[0][1] = vYAxis.x;
    this->ij[0][2] = vZAxis.x;
    this->ij[0][3] = SQFloat::_0;

    this->ij[1][0] = vXAxis.y;
    this->ij[1][1] = vYAxis.y;
    this->ij[1][2] = vZAxis.y;
    this->ij[1][3] = SQFloat::_0;

    this->ij[2][0] = vXAxis.z;
    this->ij[2][1] = vYAxis.z;
    this->ij[2][2] = vZAxis.z;
    this->ij[2][3] = SQFloat::_0;

    this->ij[3][0] = -vXAxis.DotProduct(vPointOfView);
    this->ij[3][1] = -vYAxis.DotProduct(vPointOfView);
    this->ij[3][2] = -vZAxis.DotProduct(vPointOfView);
    this->ij[3][3] = SQFloat::_1;
}

void QSpaceConversionMatrix::SetProjectionSpaceMatrix(const float_q &fNearClipPlane, const float_q &fFarClipPlane,
                                                      const float_q &fAspectRatio, const float_q &fVerticalFOV)
{
    QE_ASSERT(SQFloat::AreNotEquals(fNearClipPlane, fFarClipPlane))
    QE_ASSERT(SQFloat::IsGreaterThan(fAspectRatio, SQFloat::_0))
    QE_ASSERT(SQFloat::IsGreaterThan(fVerticalFOV, SQFloat::_0))

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        // If angles are specified in degrees, then converts it to radians
        float_q &fHalfVertFOV = SQAngle::DegreesToRadians(fVerticalFOV*SQFloat::_0_5, fHalfVertFOV);
    #else
        const float_q &fHalfVertFOV = fVerticalFOV * SQFloat::_0_5;
    #endif

    QE_ASSERT( SQFloat::AreNotEquals(fHalfVertFOV, SQAngle::_HalfPi) )

    const float_q &fScaleY  = SQFloat::_1 / tan_q(fHalfVertFOV);
    const float_q &fScaleX  = fScaleY / fAspectRatio;

    this->ij[0][0] = fScaleX;
    this->ij[0][1] = SQFloat::_0;
    this->ij[0][2] = SQFloat::_0;
    this->ij[0][3] = SQFloat::_0;
    this->ij[1][0] = SQFloat::_0;
    this->ij[1][1] = fScaleY;
    this->ij[1][2] = SQFloat::_0;
    this->ij[1][3] = SQFloat::_0;
    this->ij[2][0] = SQFloat::_0;
    this->ij[2][1] = SQFloat::_0;
    this->ij[2][2] = fFarClipPlane/(fFarClipPlane - fNearClipPlane);
    this->ij[2][3] = SQFloat::_1;
    this->ij[3][0] = SQFloat::_0;
    this->ij[3][2] = -fNearClipPlane * this->ij[2][2];
    this->ij[3][1] = SQFloat::_0;
    this->ij[3][3] = SQFloat::_0;

}

void QSpaceConversionMatrix::SwitchHandConventionViewSpaceMatrix()
{
    QBaseVector3 vPOV;
    vPOV.x = -(this->ij[3][0]*this->ij[0][0] + this->ij[3][1]*this->ij[0][1] + this->ij[3][2]*this->ij[0][2]);
    vPOV.y = -(this->ij[3][0]*this->ij[1][0] + this->ij[3][1]*this->ij[1][1] + this->ij[3][2]*this->ij[1][2]);
    vPOV.z = -(this->ij[3][0]*this->ij[2][0] + this->ij[3][1]*this->ij[2][1] + this->ij[3][2]*this->ij[2][2]);

    QBaseVector3 vUp (this->ij[0][1], this->ij[1][1], this->ij[2][1]);

    QVector3 vAux;

    QVector3 vZAxis(-this->ij[0][2], -this->ij[1][2], -this->ij[2][2]);

    QVector3 vXAxis(vUp);
    vXAxis.CrossProduct(vZAxis);

    QVector3 vYAxis(vZAxis);
    vYAxis.CrossProduct(vXAxis);

    this->ij[0][0] = vXAxis.x;
    this->ij[0][1] = vYAxis.x;
    this->ij[0][2] = vZAxis.x;
    this->ij[0][3] = SQFloat::_0;

    this->ij[1][0] = vXAxis.y;
    this->ij[1][1] = vYAxis.y;
    this->ij[1][2] = vZAxis.y;
    this->ij[1][3] = SQFloat::_0;

    this->ij[2][0] = vXAxis.z;
    this->ij[2][1] = vYAxis.z;
    this->ij[2][2] = vZAxis.z;
    this->ij[2][3] = SQFloat::_0;

    this->ij[3][0] = -vXAxis.DotProduct(vPOV);
    this->ij[3][1] = -vYAxis.DotProduct(vPOV);
    this->ij[3][2] = -vZAxis.DotProduct(vPOV);
    this->ij[3][3] = SQFloat::_1;
}

void QSpaceConversionMatrix::SetWorldSpaceMatrix(const QTranslationMatrix<QMatrix4x3> &translation, const QRotationMatrix3x3 &rotation, const QScaleMatrix3x3 &scale)
{
    SetWorldSpaceMatrixImp(translation, rotation, scale);
}

void QSpaceConversionMatrix::SetWorldSpaceMatrix(const QTranslationMatrix<QMatrix4x4> &translation, const QRotationMatrix3x3 &rotation, const QScaleMatrix3x3 &scale)
{
    SetWorldSpaceMatrixImp(translation, rotation, scale);
}

template <class MatrixType>
void QSpaceConversionMatrix::SetWorldSpaceMatrixImp(const QTranslationMatrix<MatrixType> &translation, const QRotationMatrix3x3 &rotation, const QScaleMatrix3x3 &scale)
{
    QTransformationMatrix<QMatrix4x4> aux(translation, rotation, scale);

    *this = aux.As<QSpaceConversionMatrix>();
}

void QSpaceConversionMatrix::SwitchHandConventionWorldSpaceMatrix()
{
    this->As<QTransformationMatrix<QMatrix4x4> >().SwitchHandConvention();
}

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis