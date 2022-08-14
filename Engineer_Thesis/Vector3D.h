#pragma once
#include <iostream>
class Vector3D {

public:

	float x;
	float y;
	float z;

	//default constructor
	Vector3D();
	//constructor with x, y and z values
	Vector3D(float x, float y, float z);

	//vector operations: add two vectors: v1.add(v2) equals to v1+v2 etc.
	Vector3D& Add(const Vector3D& vect);
	Vector3D& Subtract(const Vector3D& vect);
	Vector3D& Multiply(const Vector3D& vect);
	Vector3D& Divide(const Vector3D& vect);

	//vector +,-,*,/ operators overloading:
	friend Vector3D& operator+ (Vector3D& v1, const Vector3D& v2);
	friend Vector3D& operator- (Vector3D& v1, const Vector3D& v2);
	friend Vector3D& operator* (Vector3D& v1, const Vector3D& v2);
	friend Vector3D& operator/ (Vector3D& v1, const Vector3D& v2);

	//vector +=, -+, *=, /= operators overloading:
	Vector3D& operator+=(const Vector3D& vect);
	Vector3D& operator-=(const Vector3D& vect);
	Vector3D& operator*=(const Vector3D& vect);
	Vector3D& operator/=(const Vector3D& vect);

	//multiply our vector by scale
	Vector3D& operator*(const int& i);

	//zero our vector
	Vector3D& Zero();

	//overload << opearator to display vectors:
	friend std::ostream& operator << (std::ostream& output, const Vector3D& vect);

};
