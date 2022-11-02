#pragma once
#include "Vehicle.h"
#include "Planet.h"
#include <fstream>

//forward declare class
class EngineTimes;

class Solver {

public:
	const double G = 667259.0 / 10000000000000000.0;
	std::vector <double> time_data, mass_data, fuel_data;
	std::vector <Vector3D> position_data, velocity_data, engine_data, force_data, kinetic_data, potential_data;
	std::vector <Planet> Planets;
	std::vector <EngineTimes> TimeVect;
	Point_Particle Particle = Point_Particle("", 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//T is time of simulation, t is time step
	double T = 0, t = 0;

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
	void Push_Back(double time);
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
class EngineTimes : public Solver {

public:
	Vector3D timestart= { 0,0,0 }, timeend = { 0,0,0 }, engforce = { 0,0,0 };
	
	EngineTimes() {};

	void Print_Interval() {
		std::cout << "\nStart times(x,y,z): " << timestart << " s"
			<< "\nEnd times(x,y,z): " << timeend << " s"
			<< "\nEngine force(x,y,z): " << engforce << " N" << std::endl;
	}
	bool Check_input() {
		//check input:
			//initial check:
		if (TimeVect.empty()) {
			//timestart 
			if (timestart.x < 0 || timestart.y < 0 || timestart.z < 0) return false;
			//timeend
			if (timeend.x > T || timeend.y > T || timeend.z > T) return false;
		}
		else {
			if (timestart.x < 0 || timestart.y < 0 || timestart.z < 0) return false;
			if (timeend.x > T || timeend.y > T || timeend.z > T) return false;
			//check if interval does not intersect previous interval
			if (timestart.x < TimeVect.back().timestart.x || timestart.y < TimeVect.back().timestart.y || timestart.z < TimeVect.back().timestart.z) return false;
			if (timeend.x < TimeVect.back().timeend.x || timeend.y < TimeVect.back().timeend.y || timeend.z < TimeVect.back().timeend.z) return false;
		}
		//if all conditions are satisfied return true
		return true;
	}
};