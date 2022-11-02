#pragma once
#include "Vehicle.h"
#include "Planet.h"
#include <fstream>

//forward declare class
class Control;

class Solver {

public:
	const double G = 667259.0 / 10000000000000000.0;
	std::vector <double> time_data, mass_data, fuel_data;
	std::vector <Vector3D> position_data, velocity_data, engine_data, force_data, kinetic_data, potential_data;
	std::vector <Planet> Planets;
	std::vector <Control> TimeVect;
	Point_Particle Particle = Point_Particle("", 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//T is time of simulation, step is time step and time is current simulation time
	double T = 0, step = 0, time = 0;

	//save intial values to corresponding vectors
	void Write_initial();
	//define planets in simulation
	void Populate();
	//Create all simulation elements
	void Setup();
	//Save simulation elements as a Json file
	void Save_json();
	//display files in directory and open
	std::ifstream Load_file(std::string sys_path, std::string filepath, std::string extenstion);
	//load data from json format
	void Load_data(std::string& filename);
	//check if there are collistions
	bool Check_Collision(Planet& Planet);
	//function to check intervals and apply engine force
	bool UseEngine(double& time);
	//Solver using Euler method
	void Euler();
	//save values to vectors
	void Push_Back();
	//Save simulation data to file
	void Save_data();
	//Save planet configuration to a file
	void Save_planets();
	//check if file is empty 
	bool is_empty(std::ifstream& pFile)
	{
		return pFile.peek() == std::ifstream::traits_type::eof();
	}
};

//class that will hold time intervals when forces are used;
class Control{

public:
	Vector3D timestart= { 0,0,0 }, timeend = { 0,0,0 }, engforce = { 0,0,0 };
	
	Control() {};

	void Print_Interval() {
		std::cout << "\nStart times(x,y,z): " << timestart << " s"
			<< "\nEnd times(x,y,z): " << timeend << " s"
			<< "\nEngine force(x,y,z): " << engforce << " N" << std::endl;
	}
	bool Check_input(Solver& solver) {
		//check input:
			//initial check:
		if (solver.TimeVect.empty()) {
			//timestart 
			if (timestart.x < 0 || timestart.y < 0 || timestart.z < 0) return false;
			//timeend
			if (timeend.x > solver.T || timeend.y > solver.T || timeend.z > solver.T) return false;
			if (timeend.x < timestart.x || timeend.y < timestart.y || timeend.z < timestart.z) return false;
		}
		else {
			if (timestart.x < 0 || timestart.y < 0 || timestart.z < 0) return false;
			if (timeend.x > solver.T || timeend.y > solver.T || timeend.z > solver.T) return false;
			if (timeend.x < timestart.x || timeend.y < timestart.y || timeend.z < timestart.z) return false;
			//check if interval does not intersect previous interval
			if (timestart.x < solver.TimeVect.back().timeend.x || timestart.y < solver.TimeVect.back().timeend.y || timestart.z < solver.TimeVect.back().timeend.z) return false;
		}
		//if all conditions are satisfied return true
		return true;
	}
};