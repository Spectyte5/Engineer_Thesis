#pragma once
#include <iostream>

/// Class for Three-Dimensional Vectors
/// 
/// This class is used for operations and storing parameters of the three-dimensional vectors
class Vector3D {

public:
	/// x component of the vector
	double x;
	/// y component of the vector
	double y;
	/// z component of the vector
	double z;

	/// default constructor
	Vector3D();
	/// constructor with x, y and z values
	Vector3D(double x, double y, double z);

	/// Add two vectors
	///
	/// ### Example
	/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// v1.Add(v2) // which equals to v1 + v2
	/// ~~~~~~~~~~~~~~~~~~~~~~
	/// @param vect is vector being added to the vector calling this method
	/// @returns Vector3D that is a vector on which method was called with vect value added to it
	Vector3D& Add(const Vector3D& vect);
	/// Substact two vectors
    ///
	/// ### Example
	/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// v1.Substract(v2) // which equals to v1 - v2
	/// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param vect is vector being substracted from the vector calling this method
    /// @returns Vector3D that is a vector on which method was called with vect value substracted from it
	Vector3D& Subtract(const Vector3D& vect);
	/// Multiply two vectors
    ///
    /// ### Example
	/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// v1.Multiply(v2) // which equals to v1 * v2
	/// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param vect is vector which the vector calling this method is multiplyied by
    /// @returns Vector3D that is a vector on which method was called multiplyied by vect value
	Vector3D& Multiply(const Vector3D& vect);
	/// Divide two vectors
	///
	/// ### Example
	/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// v1.Divide(v2) // which equals to v1 / v2
	/// ~~~~~~~~~~~~~~~~~~~~~~
	/// @param vect is vector which the vector calling this method is divided by
	/// @returns Vector3D that is a vector on which method was called divided by vect value
	Vector3D& Divide(const Vector3D& vect);

	/// Add two vectors with + operator
    ///
    /// ### Example
    /// ~~~~~~~~~~~~~~~~~~~~~~.cpp
    /// v1 + v2 
    /// ~~~~~~~~~~~~~~~~~~~~~~
	/// @param v1 is vector which we are adding into
    /// @param v2 is vector being added
    /// @returns v1 increased by v2 value
	friend Vector3D& operator+ (Vector3D& v1, const Vector3D& v2);
	/// Substract two vectors with - operator
    ///
    /// ### Example
    /// ~~~~~~~~~~~~~~~~~~~~~~.cpp
    /// v1 - v2 
    /// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param v1 is vector which we are substracting from
    /// @param v2 is vector being substracted
    /// @returns v1 decreased by v2 value
	friend Vector3D& operator- (Vector3D& v1, const Vector3D& v2);
	/// Multiply two vectors with * operator
    ///
    /// ### Example
    /// ~~~~~~~~~~~~~~~~~~~~~~.cpp
    /// v1 * v2 
    /// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param v1 is vector multiplyied
    /// @param v2 is vector we are multiplying by
    /// @returns v1 multiplied by v2 value
	friend Vector3D& operator* (Vector3D& v1, const Vector3D& v2);
	/// Divide two vectors with / operator
    ///
    /// ### Example
    /// ~~~~~~~~~~~~~~~~~~~~~~.cpp
    /// v1 / v2 
    /// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param v1 is vector divided
    /// @param v2 is vector we are dividing by
    /// @returns v1 divided by v2 value
	friend Vector3D& operator/ (Vector3D& v1, const Vector3D& v2);

	/// Add two vectors with += operator
	///
	/// ### Example
	/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// v1 += v2 
	/// ~~~~~~~~~~~~~~~~~~~~~~
	/// @param v1 is vector which we are adding into
	/// @param v2 is vector being added
	/// @returns v1 increased by v2 value
	Vector3D& operator+=(const Vector3D& vect);
	/// Substract two vectors with -= operator
    ///
    /// ### Example
    /// ~~~~~~~~~~~~~~~~~~~~~~.cpp
    /// v1 -= v2 
    /// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param v1 is vector which we are substracting from
    /// @param v2 is vector being substracted
    /// @returns v1 decreased by v2 value
	Vector3D& operator-=(const Vector3D& vect);
	/// Multiply two vectors with *= operator
    ///
    /// ### Example
    /// ~~~~~~~~~~~~~~~~~~~~~~.cpp
    /// v1 *= v2 
    /// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param v1 is vector multiplyied
    /// @param v2 is vector we are multiplying by
    /// @returns v1 multiplied by v2 value
	Vector3D& operator*=(const Vector3D& vect);
	/// Divide two vectors with /= operator
    ///
    /// ### Example
    /// ~~~~~~~~~~~~~~~~~~~~~~.cpp
    /// v1 /= v2 
    /// ~~~~~~~~~~~~~~~~~~~~~~
    /// @param v1 is vector divided
    /// @param v2 is vector we are dividing by
    /// @returns v1 divided by v2 value
	Vector3D& operator/=(const Vector3D& vect);

	/// Multiply vector by scale
	///
	/// @param i is integer value by which we want to multiply our vector
	Vector3D& operator*(const int& i);

	///Sets values of the x,y,z to 0
	Vector3D& Zero();

	/// overload of << opearator for printing vectors
	/// 
	/// @param output is ofstream where we will print data
	/// @param vect is a vector being printed
	friend std::ostream& operator << (std::ostream& output, const Vector3D& vect);

	/// Function checking if two vectors have the same x,y,z components
	/// 
	/// @param vect is vector compared to the vector calling this method
	/// @returns true if two vectors are the same, false if not
	bool VectorsEqual(const Vector3D& vect);
};
