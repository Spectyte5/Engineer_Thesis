#include "vector3D.h"

Vector3D::Vector3D() {

	x = 0;
	y = 0;
	z = 0;
}

Vector3D::Vector3D(float x, float y, float z) {

	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D& Vector3D::Add(const Vector3D& vect) {

	this->x += vect.x;
	this->y += vect.y;
	this->z += vect.z;

	//return vect
	return *this;
}
Vector3D& Vector3D::Subtract(const Vector3D& vect) {

	this->x -= vect.x;
	this->y -= vect.y;
	this->z -= vect.z;

	return *this;
}
Vector3D& Vector3D::Multiply(const Vector3D& vect) {

	this->x *= vect.x;
	this->y *= vect.y;
	this->z *= vect.z;

	return *this;
}
Vector3D& Vector3D::Divide(const Vector3D& vect) {

	this->x /= vect.x;
	this->y /= vect.y;
	this->z /= vect.z;

	return *this;
}

Vector3D& operator+ (Vector3D& v1, const Vector3D& v2) {

	//return v1 + v2
	return v1.Add(v2);
}

Vector3D& operator- (Vector3D& v1, const Vector3D& v2) {

	return v1.Subtract(v2);
}

Vector3D& operator* (Vector3D& v1, const Vector3D& v2) {

	return v1.Multiply(v2);
}

Vector3D& operator/ (Vector3D& v1, const Vector3D& v2) {

	return v1.Divide(v2);
}

Vector3D& Vector3D::operator +=(const Vector3D& vect) {

	//return vect + vect
	return this->Add(vect);
}

Vector3D& Vector3D::operator -=(const Vector3D& vect) {

	return this->Subtract(vect);
}

Vector3D& Vector3D::operator *=(const Vector3D& vect) {

	return this->Multiply(vect);
}

Vector3D& Vector3D::operator /=(const Vector3D& vect) {

	return this->Divide(vect);
}

std::ostream& operator << (std::ostream& output, const Vector3D& vect) {

	output << "(" << vect.x << "," << vect.y << "," << vect.z << ")";
	return output;
}

Vector3D& Vector3D::operator*(const int& i) {

	this->x *= i;
	this->y *= i;
	this->z *= i;

	return *this;
}

Vector3D& Vector3D::Zero() {

	this->x = 0;
	this->y = 0;
	this->z = 0;

	return *this;
}