#pragma once
#include "Vehicle.h"
#include "Planet.h"

class Solver {

public:
	const double G = 667259.0 / 10000000000000000.0;
	std::vector <double> time_data, mass_data, fuel_data;
	std::vector <Vector3D> position_data, velocity_data, force_data, kinetic_data, potential_data;
	std::vector <Planet> Planets;
	Point_Particle Particle = Point_Particle("", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//T is time of simulation, t is time step, and tx, ty, tz are times when enginges will be turned on.
	double T = 0, t = 0, tx = 0, ty = 0, tz = 0;

	//define planets in simulation
	void Populate();
	//Setup simulation
	void Setup();
	//Solver using Euler method
	void Euler();
	//Save simulation data to file
	void Save_data();
	//Load simulation data from file
	void Load_data();
	//Change data of the loaded simulation
	void Change_data();
	//Save planet configuration to a file
	void Save_planets();
	//Load planet configuration from a file
	void Load_planets();
	////Change planet configuration
	void Change_planets();
	//save values to vectors
	void Push_Back(double time);
	//overload if i need to pass each parameter seperately
	void Push_Back(double time, double mass, double fuel, Vector3D position, Vector3D velocity, Vector3D force, Vector3D kinetic_energy, Vector3D potential_energy);
	//check if file is empty 
	bool is_empty(std::ifstream& pFile);
};
