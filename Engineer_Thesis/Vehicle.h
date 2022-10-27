#pragma once
#include "Vector3D.h"
#include <vector>


class Vehicle {

public:
	
	std::string name;
	Vector3D position;
	Vector3D velocity;
	Vector3D engine;
	Vector3D force = { 0,0,0 };
	Vector3D displacement = { 0,0,0 };
	Vector3D PotentialEnergy = { 0,0,0 };
	Vector3D KineticEnergy;
	double mass;
	double fuel;
	double fuel_usage;
	bool CalculatedEnergy = 0;
};

//body having no size, no rotation (Point-mass)
class Point_Particle : public Vehicle {
	
public:
	//Basic Constructor
	Point_Particle() {};
	//Constructor for the point-mass
	Point_Particle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double ex, double ey, double ez, double m, double fuel, double fuel_usage);
	//Print information about a Particle
	void Print_info();
	//Allow user to set values of the particle.
	void User_set();
};


//full space craft with inertia
class Spacecraft : public Vehicle {};