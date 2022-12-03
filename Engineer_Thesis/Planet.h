#pragma once
#include "Vector3D.h"

class Planet {

public:

	std::vector <Vector3D> orb_data;
	double mass = 0, radius = 0, orb_radius = 0, ang_velocity = 0, start_ang = 0;
	Vector3D position = { 0,0,0 }, orb_pos = { 0,0,0 }; //orbit is actually only x,z so the y component will always be 0
	std::string name = "";
	bool isOrb = false;
	
	//default constructor
	Planet() {}

	void Print_info() {
		
		std::cout << "\nName: " << name << "\nMass: " << mass << " kg" << "\nRadius: " << radius << " m" << "\nPosition: " << position << " m";

		if (isOrb) {
			std::cout << "\nOrbit Radius: " << orb_radius << " m" << "\nOrbit Velocity: " << ang_velocity << " rad/s" << "\nOrbit Center: " << orb_pos << " m";
		}
		
		std::cout << "\n";
	}

	//move planet around orbit
	void Move_Planet(double time) {

		position.x = orb_pos.x + orb_radius * cos(start_ang + ang_velocity * time);
	    position.z = orb_pos.z + orb_radius * sin(start_ang + ang_velocity * time);
		orb_data.push_back(position);
	}
};