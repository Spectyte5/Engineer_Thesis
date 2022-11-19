#pragma once
#include "Vector3D.h"
#include <vector>

//body having no size, no rotation (Point-mass)
class Vehicle {
	
public:

	std::string name;
	Vector3D position, velocity, engine = { 0,0,0 }, force = { 0,0,0 }, displacement = { 0,0,0 };
	double mass = 0, fuel = 0, fuel_usage = 0, PotentialEnergy = 0, KineticEnergy;
	bool CalculatedEnergy = 0;

	//Basic Constructor
	Vehicle() {};
	//Constructor for the point-mass
	Vehicle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double m, double fuel, double fuel_usage);
	//Print information about the Ship
	void Print_info();
	//Allow user to set values of the Ship.
	void User_set();
};
