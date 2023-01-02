#include "Vector3D.h"

Vector3D::Vector3D() {

	x = 0;
	y = 0;
	z = 0;
}

Vector3D::Vector3D(double x, double y, double z) {

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

Vector3D operator+ (const Vector3D& v1, const Vector3D& v2) {

	Vector3D result;

	result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };

	return result;
}

Vector3D operator- (const Vector3D& v1, const Vector3D& v2) {

	Vector3D result;

	result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };

	return result;
}

Vector3D operator* (const Vector3D& v1, const Vector3D& v2) {

	Vector3D result;

	result = { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };

	return result;
}

Vector3D operator/ (const Vector3D& v1, const Vector3D& v2) {

	Vector3D result;

	result = { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };

	return result;
}

Vector3D& Vector3D::operator +=(const Vector3D& vect) {

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

Vector3D Vector3D::operator*(const double& d) {

	Vector3D result;

	result.x = this->x * d;
	result.y = this->y * d;
	result.z = this->z * d;

	return result;
}

Vector3D Vector3D::operator/(const double& d) {

	Vector3D result;

	result.x = this->x / d;
	result.y = this->y / d;
	result.z = this->z / d;

	return result;
}

Vector3D& Vector3D::Zero() {

	this->x = 0;
	this->y = 0;
	this->z = 0;

	return *this;
}

bool Vector3D::VectorsEqual(const Vector3D& vect) {
	if (x == vect.x && y == vect.y && z == vect.z) {
		return true;
	}
	return false;
}