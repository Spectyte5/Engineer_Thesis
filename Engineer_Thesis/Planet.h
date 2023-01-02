#pragma once
#include "Vector3D.h"

/// Class for different Planets
/// 
/// This class handles creating, moving and printing information about a planet
class Planet {

public:

	/// vector used for storing position data of orbitting planets
	std::vector <Vector3D> orb_data;
	/// mass of the planet [kg]
	double mass = 0; 
	/// radius of the planet [m]
	double radius = 0; 
	/// radius of the orbit [m]
	double orb_radius = 0; 
	/// angular velocity [rad/s]
	double ang_velocity = 0; 
	/// phase [rad]
	double start_ang = 0;

	/// position of the ship
	Vector3D position = { 0,0,0 };
	/// position of the orbit in space
	/// @note orbit is actually only x,z so the y component will always be 0
	Vector3D orb_pos = { 0,0,0 };
	/// name of the planet
	std::string name = "";
	/// Variable connected to the orbitting of planet if true it means that the Planet orbits around a given point
	bool isOrb = false;
	
	/// default constructor
	Planet() {}

	/// Function printing information about planet
	void Print_info() {
		
		std::cout << "\nName: " << name << "\nMass: " << mass << " kg" << "\nRadius: " << radius << " m" << "\nPosition: " << position << " m";

		if (isOrb) {
			std::cout << "\nOrbit Radius: " << orb_radius << " m" << "\nOrbit Velocity: " << ang_velocity << " rad/s" << "\nOrbit Center: " << orb_pos << " m";
		}
		
		std::cout << "\n";
	}
	/// Move Planet around orbit
	/// 
	/// Function moving planet around orbit with given angular velocity starting from given angle
	/// @param time is current time of simulation
	void Move_Planet(bool save, double time) {

		position.x = orb_pos.x + orb_radius * cos(start_ang + ang_velocity * time);
	    position.z = orb_pos.z + orb_radius * sin(start_ang + ang_velocity * time);

		if (save) {
			orb_data.push_back(position);
		}
	}
};