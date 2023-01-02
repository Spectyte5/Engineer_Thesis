#pragma once
#include <fstream>
#include <iomanip>
#include "Vehicle.h"
#include "Planet.h"


//forward declare class
class Control;

/// Main class, used for solving
/// 
/// Class taking care of loading, validating, saving files and calculating results using different solvers
class Solver {

public:
	///Gravitational constant
	const double G = 6.67259e-11;

	int index = 0;
	double temp_mass;
	Vector3D temp_force;

	/// vectors for storing current time value
	std::vector <double> time_data;
	/// vectors for storing current mass 
	std::vector <double> mass_data;
	/// vectors for storing current fuel value
	std::vector <double> fuel_data;
	/// vectors for storing current kinetic energy value
	std::vector <double> kinetic_data;
	/// vectors for storing current potential energy value
	std::vector <double> potential_data;
	/// vector used for storing current position data
	std::vector <Vector3D> position_data;
	/// vector used for storing current velocity data
	std::vector <Vector3D>velocity_data;
	/// vector used for storing current engine data
	std::vector <Vector3D> engine_data;
	/// vector used for storing current force data
	std::vector <Vector3D> force_data;
	/// vector storing planets in the simulation
	std::vector <Planet> Planets;
	/// vector storing force intervals of type Control
	std::vector <Control> TimeVect;
	/// Ship member used in simulation
	Vehicle Ship = Vehicle("", 0, 0, 0, 0, 0, 0, 0, 0, 0);
	/// Gravitational force and distance from Planet at the time T
	Vector3D grav_forces, distance;
	/// Boolean used for removing fuel used
	bool engine_used = false;
	/// Enum of different solving ODEs methods
	enum ode { adams, euler, midpoint, runge};
	/// Method stored as an int used for enum
	int method=0; 
	/// time of simulation
	double T = 0;
	/// time step between increments
	double step = 0;
	/// current simulation time
	double time = 0;
	/// ammount of fuel_used at the iteration
	double fuel_used = 0;
	/// Derivative of Velocity
	/// 
	/// Function returning the derivative of velocity.
	/// @param f is force at time t
	/// @param m is mass of the object
    /// @returns Vector3D discreibing velocity change (acceleration) in the last interval
	Vector3D dvdt(Vector3D f, double m) { return f/m; }
	/// Derivative of Position
	/// 
	/// Function returning the derivative of position.
	/// @param v is velocity at time t
	/// @returns Vector3D discreibing position (velocity) change in the last interval
	Vector3D dxdt(Vector3D v) { return v; }
	///Define planets in simulation.
	///
	///Gets ammount of planets in simulation, sets parameters for planet and puts it in the planets vector 
	void Populate();
	///Create all simulation elements
	///
	/// Setup Particle and planets in simulation, fill all engine intervals
	void Setup();
	/// Json Validation function
	/// 
	/// Check if vector file validates against the schema
	/// @param filename is a filepath for the json file that will be validated
	bool Validate_Json(std::string& filename);
	///Save simulation as a Json file
	///
	/// Function used in create a simulation mode to save all parameters of the ship and planets in a json file which then can be reloaded in load mode.
	void Save_json();
	/// Function for Loading file from a directory
	/// 
	/// Display files in directory and open file with a given filepath
	/// @param sys_path is directory in which we are looking for files
	/// @param filepath is path to the file
	/// @param extension is extension of the file ex. "txt"
	/// @returns loaded file as ifstream
	std::ifstream Load_file(std::string sys_path, std::string filepath, std::string extenstion);
	/// Function Setting parameters from the file
	///
	/// Loaded file is used to set paramaters
	/// @see Load_file() for information about loading file
	void Load_data(std::string& filename);
	/// Collistion checking function
	/// 
	/// Function calculating distance between the Ship and Planet.
	/// @param Planet is a Planet Class object which we are checking ships collistion with
	/// @returns true if we have a collistion and false if there not
	/// @attention If Ship is exactly on the Planet's surface it does not count as a collistion
	bool Check_Collision(Planet& Planet);
	/// Applying the thrust force from the engines
	///
	/// Checks if we are in any of intervals defined by user and if yes and fuel is available it applies engine force
	/// @returns true if engine was used and no if not
	bool UseEngine();
	/// Function for calculating gravity
	///
	/// Iterate through planets and calculate Gravitation forces acting on the ship and it's potential energy
	void Calculate_Grav();
	/// Function for Calculating Net force
	///
	/// Sets the value of net force taking in to account gravitational and thrust forces
	/// @see Calculate_Grav() and UseEngine() for more information about forces calculation
	void Calculate_Net();

	void Reset_Param();
	void Recalculate_Forces(double time, double& mass, Vector3D position, Vector3D& force);
	/// Euler method
	///
	/// Function solving and ODE using the Euler's method and setting parameters of the ship
	/// @param velocity is velocity at current time
	/// @param position is position at current time 
	/// @param force is force acting on the spaceship
	/// @param mass is mass of the spaceship
	void Euler(Vector3D& velocity, Vector3D& position, Vector3D force, double mass);
	/// Runge-Kutta IV method
    ///
    /// Function solving and ODE using the Runge-Kutta IV-order method and setting parameters of the ship
	/// @param velocity is velocity at current time
	/// @param position is position at current time 
	/// @param force is force acting on the spaceship
	/// @param mass is mass of the spaceship
	void Runge_Kutta(Vector3D& velocity, Vector3D& position, Vector3D& force, double& mass);
	/// Midpoint method
    ///
    /// Function solving and ODE using the modified Euler's method (Midpoint method) and setting parameters of the ship
	/// @param velocity is velocity at current time
	/// @param position is position at current time 
	/// @param force is force acting on the spaceship
	/// @param mass is mass of the spaceship
	void Midpoint(Vector3D& velocity, Vector3D& position, Vector3D force, double mass);
	/// Adams-Bashforth's method
    ///
    /// Function solving and ODE using the Adams-Bashforth's predictor and corrector method for calculating and setting parameters of the ship
	/// @param velocity is velocity at current time
	/// @param position is position at current time 
	/// @param force is force acting on the spaceship
	/// @param mass is mass of the spaceship
	void Adams_Bashford(Vector3D& velocity, Vector3D& position, Vector3D& force, double& mass);
	/// Main solving function
	///
	/// This function loops through time interval calling all functions used for calculation and prints result on screen.
	/// @see Adams_Bashford(), Midpoint(), Euler(), Runge_Kutta() for more information about solving ODE's
	void Solve();
	/// Put all parameters in vectors
	/// 
	/// Save all neccessary values at time t into corresponding vectors
	void Push_Back();
	/// Method for changing position of orbitting planets
	///
	/// Checks if planets is orbitting around a point and if yes changes its position and saves it to vector 
	void Move_Orbit(bool save);
	/// Function for saving planets
	///
	/// Save all planets' paramaters to a seprate file
	void Save_Planets();
	/// Save simulation data
	/// 
	/// Saves all parameters and calls the function for saving planets' data.
	/// @see Save_Planets() for more information about saving planets
	void Save_data();

	/// Pauses between simulation elements printing.
	/// 
	/// Function printing '=' signs to allow better seperation between simulation elements and improve comfort of reading the text displayed. 
	void Print_Pauses() {
		std::cout  << std::setfill('=') << std::setw(120) << "\n";
	}
};

/// Class for engine intervals used in simulation
///
/// This class is used to create engine intervals, print and check input given by user for them.
/// @param timestart is Vector3D object at what time the engine will be turned on
/// @param timeend is Vector3D object at what time the engine will be turned off
/// @param engforce is Vector3D object the direction and magnitude of thrust force vector
/// @see Vector3D for information about three-dimenstional vectors class
class Control{

public:
	Vector3D timestart= { 0,0,0 }, timeend = { 0,0,0 }, engforce = { 0,0,0 };
	
	Control() {};

	void Print_Interval() {
		std::cout << "\nStart times(x,y,z): " << timestart << " s"
			<< "\nEnd times(x,y,z): " << timeend << " s"
			<< "\nEngine force(x,y,z): " << engforce << " N" << std::endl;
	}
	/// Function for checking if intervals given by user are possible to be implemented.
	///
	/// Iterates through all intervals given by the user, checks if the engine start, end and thrust values are correct and don't intersect each other.
	///@returns true if all conditions are satisfied
	bool Check_input(Solver& method) {
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
		return true;
	}
};