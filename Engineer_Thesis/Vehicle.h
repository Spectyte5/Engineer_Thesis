#pragma once
#include "Vector3D.h"

class Vehicle {

public:
	
	std::string name;
	Vector3D position;
	Vector3D velocity;
	Vector3D force;
	Vector3D displacement = { 0,0,0 };
	double mass;
};

//body having no size, no rotation (Point-mass)
class Point_Particle : public Vehicle {

public:
	//Constructor for the point-mass
	Point_Particle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double fx, double fy, double fz, double m);
	//Print information about a Particle
	void Print_info();
	//Solver for now using Euler method
	void Euler();
	void User_set();
};


//full space craft with inertia
class Spacecraft : public Vehicle {};