#pragma once
#include "Vector3D.h"

class Vehicle {

public:
	
	std::string name;
	Vector3D position;
	Vector3D velocity;
	Vector3D force;
	double mass;
};

//body having no size, no rotation (Point-mass)
class Point_Particle : public Vehicle {

public:
	//Constructor for the point-mass
	Point_Particle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double fx, double fy, double fz, double m);
	void Print_info();
};


//full space craft with inertia
class Spacecraft : public Vehicle {};