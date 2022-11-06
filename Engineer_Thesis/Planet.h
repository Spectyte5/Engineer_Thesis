#pragma once
#include "Vector3D.h"

class Planet {

public:

	double mass = 0;
	double radius = 0;
	Vector3D position = { 0,0,0 };
	std::string name = "";
	
	//default 
	Planet() {}

	//object constructor
	Planet(double m, double r, double x, double y, double z, std::string n) {
		mass = m;
		radius = r;
		name = n;
	}

	void Print_info(){
		std::cout << "\nName: " << name << "\nMass: " << mass << "\nRadius: " << radius  << "\nPosition: " << position << std::endl;
	}
};