#pragma once
#include <fstream>
#include <iomanip>
#include "Vehicle.h"
#include "Planet.h"


//forward declare class
class Control;

class Solver {

public:
	const double G = 667259.0 / 10000000000000000.0;
	std::vector <double> time_data, mass_data, fuel_data;
	std::vector <Vector3D> position_data, velocity_data, engine_data, force_data, kinetic_data, potential_data;
	std::vector <Planet> Planets;
	std::vector <Control> TimeVect;
	Vehicle Ship = Vehicle("", 0, 0, 0, 0, 0, 0, 0, 0, 0);
	Vector3D grav_forces, distance;
	bool engine_used = false;
	enum ode { adams, euler, midpoint, runge};
	int method=0; //which method will be used

	//T is time of simulation, step is time step and time is current simulation time
	double T = 0, step = 0, time = 0, fuel_used = 0;

	//derivatives of position and velocity
	double dvdt(double t, double v, double f, double m) { return f / m; }
	double dxdt(double t, double v, double x) { return v; }
	//define planets in simulation
	void Populate();
	//Create all simulation elements
	void Setup();
	//validate json files to a schema
	bool Validate_Json(std::string& filename);
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
	//Calculate Gravitation forces from planets and Potential Energy of particle
	void Calculate_Grav();
	//Calculate Net force
	void Calculate_Net();
	//Solver use Euler method
	void Euler(double& time, double& velocity, double& position, double& dt, double& force, double& mass);
	//Solver use Runge-Kutta IV-order method
	void Runge_Kutta(double& time, double& velocity, double& position, double& dt, double& force, double& mass);
	//Solver use Midpoint method
	void Midpoint(double& time, double& velocity, double& position, double& dt, double& force, double& mass);
	//Solver use Adam's Bashforth
	void Adams_Bashford(double& time, double& velocity, double& position, double& dt, double& force, double& mass);
	//main solving function
	void Solve();
	//save values to vectors
	void Push_Back();
	//Save simulation data to file
	void Save_data();
	//check if file is empty 
	bool is_empty(std::ifstream& pFile)
	{
		return pFile.peek() == std::ifstream::traits_type::eof();
	}
	void Print_Pauses() {
		std::cout  << std::setfill('=') << std::setw(120) << "\n";
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
	bool Check_input(Solver& method) {
		//check input:
			//initial check:
		if (method.TimeVect.empty()) {
			//timestart 
			if (timestart.x < 0 || timestart.y < 0 || timestart.z < 0) return false;
			//timeend
			if (timeend.x > method.T || timeend.y > method.T || timeend.z > method.T) return false;
			if (timeend.x < timestart.x || timeend.y < timestart.y || timeend.z < timestart.z) return false;
		}
		else {
			if (timestart.x < 0 || timestart.y < 0 || timestart.z < 0) return false;
			if (timeend.x > method.T || timeend.y > method.T || timeend.z > method.T) return false;
			if (timeend.x < timestart.x || timeend.y < timestart.y || timeend.z < timestart.z) return false;
			//check if interval does not intersect previous interval
			if (timestart.x < method.TimeVect.back().timeend.x || timestart.y < method.TimeVect.back().timeend.y || timestart.z < method.TimeVect.back().timeend.z) return false;
		}
		//if all conditions are satisfied return true
		return true;
	}
};