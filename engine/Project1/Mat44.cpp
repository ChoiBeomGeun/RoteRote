////#include "Mat44.h"
//#include "TUMath.h"
//#include <string>
////#include "Camera.h"
//
//Mat4::Mat4()
//{
//	memset(this->m, 0, sizeof(float) * 16);
//	m[0] = 1;
//	m[5] = 1;
//	m[10] = 1;
//	m[15] = 1;
//}
//
//Mat4::Mat4(float m00, float m01, float m02, float m03,
//	float m04, float m05, float m06, float m07,
//	float m08, float m09, float m10, float m11,
//	float m12, float m13, float m14, float m15)
//{
//	m[0] = m00; m[4] = m04; m[8] = m08; m[12] = m12;
//	m[1] = m01; m[5] = m05; m[9] = m09; m[13] = m13;
//	m[2] = m02; m[6] = m06; m[10] = m10; m[14] = m14;
//	m[3] = m03; m[7] = m07; m[11] = m11; m[15] = m15;
//}
//
//Mat4::Mat4(const float * m)
//{
//	memcpy(this->m, m, sizeof(float) * 16);
//}
//
//Mat4::Mat4(const Mat4 & copy)
//{
//	memcpy(this, &copy, sizeof(float) * 16);
//}
//
//void Mat4::Set(float m00, float m01, float m02, float m03,
//	float m04, float m05, float m06, float m07,
//	float m08, float m09, float m10, float m11,
//	float m12, float m13, float m14, float m15)
//{
//	m[0] = m00; m[4] = m04; m[8] = m08; m[12] = m12;
//	m[1] = m01; m[5] = m05; m[9] = m09; m[13] = m13;
//	m[2] = m02; m[6] = m06; m[10] = m10; m[14] = m14;
//	m[3] = m03; m[7] = m07; m[11] = m11; m[15] = m15;
//}
//
//void Mat4::Set(const float * f)
//{
//	for (int i = 0; i < 15; ++i)
//		m[0] = *f;
//}
//
//void Mat4::Set(const Mat4 & copy)
//{
//
//	memcpy(this, &copy, sizeof(float) * 16);
//}
//
//void Mat4::SetZero()
//{
//	memset(this->m, 0, sizeof(float) * 16);
//}
//
//void Mat4::SetIdentity()
//{
//	memset(this->m, 0, sizeof(float) * 16);
//	m[0] = 1;
//	m[5] = 1;
//	m[10] = 1;
//	m[15] = 1;
//}
//
//bool Mat4::IsZero() const
//{
//	for (int i = 0; i < 16; i++)
//	{
//		if (m[i] != 0)
//			return false;
//	}
//	return true;
//}
//
//bool Mat4::IsIdentity() const
//{
//	Mat4 I;
//	for (int i = 0; i < 16; i++)
//	{
//		if (m[i] != I.m[i])
//			return false;
//	}
//	return true;
//}
//
//bool Mat4::Inverse()
//{
//	float cf[16];
//	/* col-major
//	0   4   8   12
//	1   5   9   13
//	2   6   10  14
//	3   7   11  15*/
//
//	// Calculate the Matrix of Minors and change to the cofactors
//	cf[0] = GetMinorMat(5, 9, 13, 6, 10, 14, 7, 11, 15);
//	cf[4] = -GetMinorMat(1, 9, 13, 2, 10, 14, 3, 11, 15);
//	cf[8] = GetMinorMat(1, 5, 13, 2, 6, 14, 3, 7, 15);
//	cf[12] = -GetMinorMat(1, 5, 9, 2, 6, 10, 3, 7, 11);
//
//	// Get Determinant
//	float det = m[0] * cf[0] + m[4] * cf[4] + m[8] * cf[8] + m[12] * cf[12];
//
//	if (std::fabs(det) == 0.f)
//		return false;
//
//	/* col-major
//	0   4   8   12
//	1   5   9   13
//	2   6   10  14
//	3   7   11  15*/
//	// Calculate the rest of the Matrix of Minors and change to the cofactors
//	cf[1] = -GetMinorMat(4, 8, 12, 6, 10, 14, 7, 11, 15);
//	cf[5] = GetMinorMat(0, 8, 12, 2, 10, 14, 3, 11, 15);
//	cf[9] = -GetMinorMat(0, 4, 12, 2, 6, 14, 3, 7, 15);
//	cf[13] = GetMinorMat(0, 4, 8, 2, 6, 10, 3, 7, 11);
//
//	cf[2] = GetMinorMat(4, 8, 12, 5, 9, 13, 7, 11, 15);
//	cf[6] = -GetMinorMat(0, 8, 12, 1, 9, 13, 3, 11, 15);
//	cf[10] = GetMinorMat(0, 4, 12, 1, 5, 13, 3, 7, 15);
//	cf[14] = -GetMinorMat(0, 4, 8, 1, 5, 9, 3, 7, 11);
//
//	cf[3] = -GetMinorMat(4, 8, 12, 5, 9, 13, 6, 10, 14);
//	cf[7] = GetMinorMat(0, 8, 12, 1, 9, 13, 2, 10, 14);
//	cf[11] = -GetMinorMat(0, 4, 12, 1, 5, 13, 2, 6, 14);
//	cf[15] = GetMinorMat(0, 4, 8, 1, 5, 9, 2, 6, 10);
//
//	Set(cf);
//
//	// Transpose (Adjugate ,which is also called Adjoint)
//	this->Transpose();
//
//	// Multiply by 1 / determinant
//	*this /= det;
//
//	return true;
//	return false;
//}
//
//Mat4 Mat4::GetInversed() const
//{
//	return Mat4();
//}
//
//void Mat4::Transpose()
//{
//	std::swap(m[1], m[4]);
//	std::swap(m[2], m[8]);
//	std::swap(m[3], m[12]);
//	std::swap(m[6], m[9]);
//	std::swap(m[7], m[13]);
//	std::swap(m[11], m[14]);
//}
//
//Mat4 Mat4::GetTransposed() const
//{
//	Mat4 result;
//	result.Transpose();
//	return result;
//}
//
//void Mat4::Transform(const Vector3 & v)
//{
//
//}
//
//void Mat4::Translate(const Vector3 & translation)
//{
//	m[3] = translation.x;
//	m[7] = translation.y;
//	m[11] = translation.z;
//}
//
//void Mat4::Translate(float xTranslation, float yTranslation, float zTranslation)
//{
//	m[3] = xTranslation;
//	m[7] = yTranslation;
//	m[11] = zTranslation;
//}
//
//void Mat4::Scale(const Vector3 & scale)
//{
//	m[0] = scale.x;
//	m[5] = scale.y;
//	m[10] = scale.z;
//}
//
//void Mat4::Scale(float scale)
//{
//	m[0] = scale;
//	m[5] = scale;
//	m[10] = scale;
//}
//
//void Mat4::Scale(float xScale, float yScale, float zScale)
//{
//	m[0] = xScale;
//	m[5] = yScale;
//	m[10] = zScale;
//}
//
//void Mat4::Rotate(const Vector3 & axis, float angle)
//{
//
//	m[0] = (tan(angle)*axis.x*axis.x) + cos(angle);
//	m[4] = (tan(angle)*axis.x*axis.y) + (sin(angle)*axis.z);
//	m[8] = (tan(angle)*axis.x*axis.z) - (sin(angle)*axis.y);
//	m[1] = (tan(angle)*axis.x*axis.y) - (sin(angle)*axis.z);
//	m[5] = (tan(angle)*axis.y*axis.y) + cos(angle);
//	m[9] = (tan(angle)*axis.y*axis.z) + (sin(angle)*axis.x);
//	m[2] = (tan(angle)*axis.x*axis.z) + (sin(angle)*axis.y);
//	m[6] = (tan(angle)*axis.y*axis.z) - (sin(angle)*axis.x);
//	m[10] = (tan(angle)*axis.z*axis.z) + (cos(angle));
//}
//
//void Mat4::RotateX(float angle)
//{
//	m[5] = cos(angle);
//	m[9] = -sin(angle);
//	m[6] = sin(angle);
//	m[10] = cos(angle);
//}
//
//void Mat4::RotateY(float angle)
//{
//	m[0] = cos(angle);
//	m[8] = sin(angle);
//	m[2] = -sin(angle);
//	m[10] = cos(angle);
//}
//
//void Mat4::RotateZ(float angle)
//{
//	m[0] = cos(angle);
//	m[4] = -sin(angle);
//	m[1] = sin(angle);
//	m[5] = cos(angle);
//}
//
//Mat4 Mat4::perspective(const float angleofView, const float aspect, const float near, const float far)
//{
//	// set the basic projection matrix
//	float scale = tanf(TUMath::DegreeToRadian(angleofView*.5f));
//	Mat4 m;
//	m.m[0] = 1 / (scale * aspect); // scale the x coordinates of the projected point
//	m.m[5] = 1 / scale; // scale the y coordinates of the projected point
//	m.m[10] = -(far + near) / (far - near); // used to remap z to [0,1]
//	m.m[14] = -2 * (far * near) / (far - near); // used to remap z [0,1]
//	m.m[11] = -1; // set w = -z
//	m.m[15] = 0;
//	return m;
//}
//Mat4 Mat4::ortho(const float left, const float right, const float bottom, const float top)
//{
//	Mat4 result;
//	result.SetIdentity();
//	result.m[0] = 2 / (right - left);
//	result.m[5] = 2 / (top - bottom);
//	result.m[10] = -1;
//	result.m[12] = -(right + left) / (right - left);
//	result.m[13] = -(top + bottom) / (top - bottom);
//
//	return result;
//}
//Mat4 Mat4::lookAt(const Vector3 & eye, const Vector3& target, const Vector3& up)
//{
//	Vector3 zaxis = Vector3::Normalize(eye - target);
//	Vector3 yaxis = up;
//	Vector3 xaxis = Vector3::Normalize(Vector3::CrossProduct(yaxis, zaxis));
//	yaxis = Vector3::CrossProduct(zaxis, xaxis);
//
//	// create a 4X4 orientation matrix from the right, up, and forward vectors
//	// this is transposed which is equivalent to performing an inverse
//	// if the matrix is orthonormalized 
//	Mat4 orientation;
//	orientation.Set(xaxis.x, xaxis.y, xaxis.z, xaxis.DotProduct(-eye),
//		yaxis.x, yaxis.y, yaxis.z, yaxis.DotProduct(-eye),
//		zaxis.x, zaxis.y, zaxis.z, zaxis.DotProduct(-eye),
//		0, 0, 0, 1);
//
//	// combine the orientation and translation to compute 
//	// the final view matrix
//	/*Mat4 result; result.SetIdentity();
//	result = (orientation * translation);
//	*/
//	orientation.Transpose();
//	return orientation;
//}
//
//Mat4 Mat4::Camera(const Vector3 & eye, const Vector3 & target, const Vector3 & up)
//{
//	Mat4 camera;
//	camera = camera.lookAt(eye, target, up);
//	return camera;
//}
//
//Mat4 Mat4::GetTranslation(const Vector3 & translation)
//{
//	Mat4 result;
//	result.Translate(translation);
//	return result;
//}
//
//Mat4 Mat4::GetTranslation(float xTranslation, float yTranslation, float zTranslation)
//{
//	Mat4 result;
//	result.Translate(xTranslation, yTranslation, zTranslation);
//	return result;
//}
//
//Mat4 Mat4::GetScaling(const Vector3 & scale)
//{
//	Mat4 result;
//	result.Scale(scale.x, scale.y, scale.z);
//	return result;
//}
//
//Mat4 Mat4::GetScaling(float scale)
//{
//	Mat4 result;
//	result.Scale(scale);
//	return result;
//}
//
//Mat4 Mat4::GetScaling(float xScale, float yScale, float zScale)
//{
//	Mat4 result;
//	result.Scale(xScale, yScale, zScale);
//	return result;
//}
//
//Mat4 Mat4::GetRotation(const Vector3 & axis, float angle)
//{
//	Mat4 result;
//	result.Rotate(axis, angle);
//	return result;
//}
//
//Mat4 Mat4::GetRotationX(float angle)
//{
//	Mat4 result;
//	result.RotateX(angle);
//	return result;
//}
//
//Mat4 Mat4::GetRotationY(float angle)
//{
//	Mat4 result;
//	result.RotateY(angle);
//	return result;
//}
//
//Mat4 Mat4::GetRotationZ(float angle)
//{
//	Mat4 result;
//	result.RotateZ(angle);
//	return result;
//}
//
//Mat4 Mat4::GetMatrixModel(const Mat4 & scale, const Mat4 & rotate, const Mat4 & translate)
//{
//	Mat4 result;
//	result.SetIdentity();
//	result = translate *(rotate*scale);
//	return result;
//}
//
//Mat4 Mat4::ModelToWorld(const Mat4 & Model)
//{
//	Mat4 World;
//
//	return Mat4();
//}
//
//Mat4 Mat4::operator+(const Mat4 & mat) const
//{
//	Mat4 mat4 = mat;
//	for (int i = 0; i < 16; i++)
//	{
//		mat4.m[i] += m[i];
//	}
//	return mat4;
//}
//
//Mat4 & Mat4::operator+=(const Mat4 & mat)
//{
//
//	for (int i = 0; i < 16; i++)
//	{
//		m[i] += mat.m[i];
//	}
//	return *this;
//}
//
//Mat4 Mat4::operator-(const Mat4 & mat) const
//{
//	Mat4 mat4 = mat;
//	for (int i = 0; i < 16; i++)
//	{
//		mat4.m[i] -= m[i];
//	}
//	return mat4;
//}
//
//Mat4 & Mat4::operator-=(const Mat4 & mat)
//{
//	for (int i = 0; i < 16; i++)
//	{
//		m[i] -= mat.m[i];
//	}
//	return *this;
//}
//
//Mat4 Mat4::operator*(const Mat4 & mat)
//{
//	float result[16];
//	/* Row major
//	result[0] = m[0] * mat.m[0] + m[1] * mat.m[4] + m[2] * mat.m[8] + m[3] * mat.m[12];
//	result[1] = m[0] * mat.m[1] + m[1] * mat.m[5] + m[2] * mat.m[9] + m[3] * mat.m[13];
//	result[2] = m[0] * mat.m[2] + m[1] * mat.m[6] + m[2] * mat.m[10] + m[3] * mat.m[14];
//	result[3] = m[0] * mat.m[3] + m[1] * mat.m[7] + m[2] * mat.m[11] + m[3] * mat.m[15];
//
//	result[4] = m[4] * mat.m[0] + m[5] * mat.m[4] + m[6] * mat.m[8] + m[7] * mat.m[12];
//	result[5] = m[4] * mat.m[1] + m[5] * mat.m[5] + m[6] * mat.m[9] + m[7] * mat.m[13];
//	result[6] = m[4] * mat.m[2] + m[5] * mat.m[6] + m[6] * mat.m[10] + m[7] * mat.m[14];
//	result[7] = m[4] * mat.m[3] + m[5] * mat.m[7] + m[6] * mat.m[11] + m[7] * mat.m[15];
//
//	result[8] = m[8] * mat.m[0] + m[9] * mat.m[4] + m[10] * mat.m[8] + m[11] * mat.m[12];
//	result[9] = m[8] * mat.m[1] + m[9] * mat.m[5] + m[10] * mat.m[9] + m[11] * mat.m[13];
//	result[10] = m[8] * mat.m[2] + m[9] * mat.m[6] + m[10] * mat.m[10] + m[11] * mat.m[14];
//	result[11] = m[8] * mat.m[3] + m[9] * mat.m[7] + m[10] * mat.m[11] + m[11] * mat.m[15];
//
//	result[12] = m[12] * mat.m[0] + m[13] * mat.m[4] + m[14] * mat.m[8] + m[15] * mat.m[12];
//	result[13] = m[12] * mat.m[1] + m[13] * mat.m[5] + m[14] * mat.m[9] + m[15] * mat.m[13];
//	result[14] = m[12] * mat.m[2] + m[13] * mat.m[6] + m[14] * mat.m[10] + m[15] * mat.m[14];
//	result[15] = m[12] * mat.m[3] + m[13] * mat.m[7] + m[14] * mat.m[11] + m[15] * mat.m[15];
//	*/
//
//	result[0] = m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
//	result[4] = m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
//	result[8] = m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
//	result[12] = m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];
//
//	result[1] = m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
//	result[5] = m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
//	result[9] = m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
//	result[13] = m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];
//
//
//	result[2] = m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
//	result[6] = m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
//	result[10] = m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
//	result[14] = m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];
//
//	result[3] = m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];
//	result[7] = m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];
//	result[11] = m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];
//	result[15] = m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];
//
//	return Mat4(result);
//}
//
//Mat4 Mat4::operator*(const Mat4 & mat) const
//{
//	float result[16];
//	/* Row major
//	result[0] = m[0] * mat.m[0] + m[1] * mat.m[4] + m[2] * mat.m[8] + m[3] * mat.m[12];
//	result[1] = m[0] * mat.m[1] + m[1] * mat.m[5] + m[2] * mat.m[9] + m[3] * mat.m[13];
//	result[2] = m[0] * mat.m[2] + m[1] * mat.m[6] + m[2] * mat.m[10] + m[3] * mat.m[14];
//	result[3] = m[0] * mat.m[3] + m[1] * mat.m[7] + m[2] * mat.m[11] + m[3] * mat.m[15];
//
//	result[4] = m[4] * mat.m[0] + m[5] * mat.m[4] + m[6] * mat.m[8] + m[7] * mat.m[12];
//	result[5] = m[4] * mat.m[1] + m[5] * mat.m[5] + m[6] * mat.m[9] + m[7] * mat.m[13];
//	result[6] = m[4] * mat.m[2] + m[5] * mat.m[6] + m[6] * mat.m[10] + m[7] * mat.m[14];
//	result[7] = m[4] * mat.m[3] + m[5] * mat.m[7] + m[6] * mat.m[11] + m[7] * mat.m[15];
//
//	result[8] = m[8] * mat.m[0] + m[9] * mat.m[4] + m[10] * mat.m[8] + m[11] * mat.m[12];
//	result[9] = m[8] * mat.m[1] + m[9] * mat.m[5] + m[10] * mat.m[9] + m[11] * mat.m[13];
//	result[10] = m[8] * mat.m[2] + m[9] * mat.m[6] + m[10] * mat.m[10] + m[11] * mat.m[14];
//	result[11] = m[8] * mat.m[3] + m[9] * mat.m[7] + m[10] * mat.m[11] + m[11] * mat.m[15];
//
//	result[12] = m[12] * mat.m[0] + m[13] * mat.m[4] + m[14] * mat.m[8] + m[15] * mat.m[12];
//	result[13] = m[12] * mat.m[1] + m[13] * mat.m[5] + m[14] * mat.m[9] + m[15] * mat.m[13];
//	result[14] = m[12] * mat.m[2] + m[13] * mat.m[6] + m[14] * mat.m[10] + m[15] * mat.m[14];
//	result[15] = m[12] * mat.m[3] + m[13] * mat.m[7] + m[14] * mat.m[11] + m[15] * mat.m[15];
//	*/
//
//	result[0] = m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
//	result[4] = m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
//	result[8] = m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
//	result[12] = m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];
//
//	result[1] = m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
//	result[5] = m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
//	result[9] = m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
//	result[13] = m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];
//
//
//	result[2] = m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
//	result[6] = m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
//	result[10] = m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
//	result[14] = m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];
//
//	result[3] = m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];
//	result[7] = m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];
//	result[11] = m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];
//	result[15] = m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];
//
//	return Mat4(result);
//}
//
//Mat4 & Mat4::operator*=(const Mat4 & mat)
//{
//	float result[16] = { 0 };
//	/* Row major
//	result[0] = m[0] * mat.m[0] + m[1] * mat.m[4] + m[2] * mat.m[8] + m[3] * mat.m[12];
//	result[1] = m[0] * mat.m[1] + m[1] * mat.m[5] + m[2] * mat.m[9] + m[3] * mat.m[13];
//	result[2] = m[0] * mat.m[2] + m[1] * mat.m[6] + m[2] * mat.m[10] + m[3] * mat.m[14];
//	result[3] = m[0] * mat.m[3] + m[1] * mat.m[7] + m[2] * mat.m[11] + m[3] * mat.m[15];
//
//	result[4] = m[4] * mat.m[0] + m[5] * mat.m[4] + m[6] * mat.m[8] + m[7] * mat.m[12];
//	result[5] = m[4] * mat.m[1] + m[5] * mat.m[5] + m[6] * mat.m[9] + m[7] * mat.m[13];
//	result[6] = m[4] * mat.m[2] + m[5] * mat.m[6] + m[6] * mat.m[10] + m[7] * mat.m[14];
//	result[7] = m[4] * mat.m[3] + m[5] * mat.m[7] + m[6] * mat.m[11] + m[7] * mat.m[15];
//
//	result[8] = m[8] * mat.m[0] + m[9] * mat.m[4] + m[10] * mat.m[8] + m[11] * mat.m[12];
//	result[9] = m[8] * mat.m[1] + m[9] * mat.m[5] + m[10] * mat.m[9] + m[11] * mat.m[13];
//	result[10] = m[8] * mat.m[2] + m[9] * mat.m[6] + m[10] * mat.m[10] + m[11] * mat.m[14];
//	result[11] = m[8] * mat.m[3] + m[9] * mat.m[7] + m[10] * mat.m[11] + m[11] * mat.m[15];
//
//	result[12] = m[12] * mat.m[0] + m[13] * mat.m[4] + m[14] * mat.m[8] + m[15] * mat.m[12];
//	result[13] = m[12] * mat.m[1] + m[13] * mat.m[5] + m[14] * mat.m[9] + m[15] * mat.m[13];
//	result[14] = m[12] * mat.m[2] + m[13] * mat.m[6] + m[14] * mat.m[10] + m[15] * mat.m[14];
//	result[15] = m[12] * mat.m[3] + m[13] * mat.m[7] + m[14] * mat.m[11] + m[15] * mat.m[15];
//	*/
//	// Column Major 
//	result[0] = m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
//	result[1] = m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
//	result[2] = m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
//	result[3] = m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];
//
//	result[4] = m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
//	result[5] = m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
//	result[6] = m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
//	result[7] = m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];
//
//
//	result[8] = m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
//	result[9] = m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
//	result[10] = m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
//	result[11] = m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];
//
//	result[12] = m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];
//	result[13] = m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];
//	result[14] = m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];
//	result[15] = m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];
//
//	for (int i = 0; i< 16; ++i)
//	{
//		m[i] = result[i];
//	}
//	//std::memcpy(m, result, 16);
//
//	return *this;
//}
//
//
//
//Mat4& Mat4::operator*(Mat4& mat)
//{
//	float result[16] = { 0 };
//	result[0] = m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
//	result[1] = m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
//	result[2] = m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
//	result[3] = m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];
//
//	result[4] = m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
//	result[5] = m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
//	result[6] = m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
//	result[7] = m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];
//
//
//	result[8] = m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
//	result[9] = m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
//	result[10] = m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
//	result[11] = m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];
//
//	result[12] = m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];
//	result[13] = m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];
//	result[14] = m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];
//	result[15] = m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];
//
//	for (int i = 0; i< 16; ++i)
//	{
//		m[i] = result[i];
//	}
//	//std::memcpy(m, result, 16);
//
//	return *this;
//
//}
//
//Mat4 Mat4::operator*(float s) const
//{
//	Mat4 mat4 = *this;
//	for (int i = 0; i < 16; i++)
//	{
//		mat4.m[i] *= s;
//	}
//	return mat4;
//}
//
//Mat4 & Mat4::operator*=(float s)
//{
//	for (unsigned i = 0; i < 15; ++i)
//		m[i] *= s;
//
//	return *this;
//}
//
//Mat4 Mat4::operator/(float s) const
//{
//	Mat4 mat4 = *this;
//	for (int i = 0; i < 16; i++)
//	{
//		mat4.m[i] /= s;
//	}
//	return mat4;
//}
//
//Mat4 & Mat4::operator/=(float s)
//{
//	for (int i = 0; i < 16; i++)
//	{
//		m[i] /= s;
//	}
//	return *this;
//}
//
//Vector3 Mat4::operator*(const Vector3 & v) const
//{
//	return Vector3(
//		m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * 1.f,
//		m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * 1.f,
//		m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * 1.f
//	);
//}
//
//bool Mat4::operator==(const Mat4 & mat) const
//{
//	for (int i = 0; i < 16; i++)
//	{
//		if (m[i] != mat.m[i])
//			return false;
//	}
//	return true;
//}
//
//bool Mat4::operator!=(const Mat4 & mat) const
//{
//	for (int i = 0; i < 16; i++)
//	{
//		if (m[i] != mat.m[i])
//			return true;
//	}
//	return false;
//}
//
//float Mat4::GetMinorMat(unsigned a0, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7, unsigned a8) const
//{
//	return m[a0] * (m[a4] * m[a8] - m[a5] * m[a7])
//		- m[a1] * (m[a3] * m[a8] - m[a5] * m[a6])
//		+ m[a2] * (m[a3] * m[a7] - m[a4] * m[a6]);
//}
//
////Mat4 operator*(Mat4& mat1, Mat4& mat2)
////{
////	
////	float m[16] = { 0 };
////	m[0] = mat1.m[0] * mat2.m[0] + mat1.m[4] * mat2.m[1] + mat1.m[8] * mat2.m[2] + mat1.m[12] * mat2.m[3];
////	m[1] = mat1.m[1] * mat2.m[0] + mat1.m[5] * mat2.m[1] + mat1.m[9] * mat2.m[2] + mat1.m[13] * mat2.m[3];
////	m[2] = mat1.m[2] * mat2.m[0] + mat1.m[6] * mat2.m[1] + mat1.m[10] * mat2.m[2] + mat1.m[14] * mat2.m[3];
////	m[3] = mat1.m[3] * mat2.m[0] + mat1.m[17] * mat2.m[1] + mat1.m[11] * mat2.m[2] + mat1.m[15] * mat2.m[3];
////
////	m[4] = mat1.m[0] * mat2.m[4] + mat1.m[4] * mat2.m[5] + mat1.m[8] * mat2.m[6] + mat1.m[12] * mat2.m[7];
////	m[5] = mat1.m[1] * mat2.m[4] + mat1.m[5] * mat2.m[5] + mat1.m[9] * mat2.m[6] + mat1.m[13] * mat2.m[7];
////	m[6] = mat1.m[2] * mat2.m[4] + mat1.m[6] * mat2.m[5] + mat1.m[10] * mat2.m[6] + mat1.m[14] * mat2.m[7];
////	m[7] = mat1.m[3] * mat2.m[4] + mat1.m[7] * mat2.m[5] + mat1.m[11] * mat2.m[6] + mat1.m[15] * mat2.m[7];
////
////
////	m[8] = mat1.m[0] * mat2.m[8] + mat1.m[4] * mat2.m[9] + mat1.m[8] * mat2.m[10] + mat1.m[12] * mat2.m[11];
////	m[9] = mat1.m[1] * mat2.m[8] + mat1.m[5] * mat2.m[9] + mat1.m[9] * mat2.m[10] + mat1.m[13] * mat2.m[11];
////	m[10] = mat1.m[2] * mat2.m[8] + mat1.m[6] * mat2.m[9] + mat1.m[10] * mat2.m[10] + mat1.m[14] * mat2.m[11];
////	m[11] = mat1.m[3] * mat2.m[8] + mat1.m[7] * mat2.m[9] + mat1.m[11] * mat2.m[10] + mat1.m[15] * mat2.m[11];
////
////	m[12] = mat1.m[0] * mat2.m[12] + mat1.m[4] * mat2.m[13] + mat1.m[8] * mat2.m[14] + mat1.m[12] * mat2.m[15];
////	m[13] = mat1.m[1] * mat2.m[12] + mat1.m[5] * mat2.m[13] + mat1.m[9] * mat2.m[14] + mat1.m[13] * mat2.m[15];
////	m[14] = mat1.m[2] * mat2.m[12] + mat1.m[6] * mat2.m[13] + mat1.m[10] * mat2.m[14] + mat1.m[14] * mat2.m[15];
////	m[15] = mat1.m[3] * mat2.m[12] + mat1.m[7] * mat2.m[13] + mat1.m[11] * mat2.m[14] + mat1.m[15] * mat2.m[15];
////
////	for (int i = 0; i< 16; ++i)
////	{
////		m[i] = m[i];
////	}
////	return m;
////
////}
//
//Vector3 & operator*=(Vector3 & v, const Mat4 & mat)
//{
//
//	float x = mat.m[0] * v.x + mat.m[4] * v.y + mat.m[8] * v.z + mat.m[12] * 1.f;
//	float y = mat.m[1] * v.x + mat.m[5] * v.y + mat.m[9] * v.z + mat.m[13] * 1.f;
//	float z = mat.m[2] * v.x + mat.m[6] * v.y + mat.m[10] * v.z + mat.m[14] * 1.f;
//
//	v.Set(x, y, z);
//
//	return v;
//}
