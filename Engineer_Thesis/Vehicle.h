#pragma once
#include "Vector3D.h"
#include <vector>

///Class for different spaceship objects
///
/// Ship is a body having no size, no rotation (Point-mass)
/// @note In few places the mass actually is a mass without fuel, inputed by user that then has the fuel mass added to it.
/// @see Vector3D for more information about three-dimenstional vector objects
class Vehicle {
	
public:
	/// name of the ship
	std::string name;
	/// Vector3D position on x,y,z axis [m]
	Vector3D position; 
	/// Vector3D velocity on x,y,z axis [m/s]
	Vector3D velocity;
	/// engine is a Vector3D thrust force on x,y,z axis [N]
	Vector3D engine = { 0,0,0 };
	/// Vector3D net force acting on spaceship on x,y,z axis [N]
	Vector3D force = { 0,0,0 };
	/// displacement of the ship from initial position[m]
	Vector3D displacement = { 0,0,0 };
	/// total mass of the ship with fuel [kg]
	double mass = 0;
	/// mass of fuel carried by the ship[kg]
	double fuel = 0;
	/// ammount of fuel used by engines [kg/s]
	double fuel_usage = 0;
	/// total potential energy from all planets acting on the spaceship [J]
	double PotentialEnergy = 0; 
	/// energy from velocity whith which spaceship is moving [J]
	double KineticEnergy = 0;
	/// true or false depending on whether the planets where already initialized
	///
	/// @see Planet more info about planets
	bool CalculatedEnergy = 0;

	///Default Constructor
	Vehicle() {};
	/// Constructor assiging given paramaters
	///
	/// @param n is name of the ship
    /// @param rx is position on x axis [m]
    /// @param ry is position on y axis [m]
    /// @param rz is position on z axis [m]
    /// @param vx is velocity on x axis [m/s]
	/// @param vy is velocity on y axis [m/s]
	/// @param vz is velocity on z axis [m/s]
	/// @param m is mass of the ship [kg]
	/// @param fuel is mass of fuel carried by the ship [kg]
	/// @param fuel_usage is ammount of fuel used in [kg/s]

	Vehicle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double m, double fuel, double fuel_usage);
	/// Print information about the Ship
	///
	/// Function for printing each paramter of the ship on screen
	void Print_info();
	/// User set ships parameters
	///
	/// Function allowing user to set values of the Ship, used in create a simulation mode.
	void User_set();
};
