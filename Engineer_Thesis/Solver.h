#pragma once
#include "Vehicle.h"

class Solver {

public:

	std::vector <double> time_data, mass_data, fuel_data;
	std::vector <Vector3D> position_data, velocity_data, force_data;

	//T is time of simulation, t is time step, and tx, ty, tz are times when enginges will be turned on.
	double T = 0, t = 0, tx = 0, ty = 0, tz = 0;

	//Solver for using Euler method
	void Euler(Point_Particle& Particle);
	//Save simulation data to file
	void Save_data(Point_Particle& Particle);
};

