#include "Vehicle.h"

Vehicle::Vehicle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double m, double f, double u) {

	//name 
	name = n;
	
	//position
	position = { rx, ry, rz };

	//velocities
	velocity = { vx, vy, vz };
	
	//mass
	mass = m;
	
	//fuel
	fuel = f;
	fuel_usage = u;

	//Energy
	KineticEnergy = mass / 2 * (pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));  //ek = 1/2 * mv^2
}

void Vehicle::Print_info() {

	std::cout << std::fixed << this->name << " Data: " << "\n mass: " << this->mass << " kg" << "\n position : "
		<< this->position << " m" << "\n displacement : " << this->displacement << " m" << "\n velocity : "
		<< this->velocity << " m/s" << "\n engine force : " << this->engine << " N" << "\n net force : " 
		<< this->force << " N" << "\n fuel mass : " << this->fuel << " kg" << "\n fuel consumption : " << this->fuel_usage << " kg/s";

	//check if potential energy can be displaced (Are planets initialzed?)	
	if (CalculatedEnergy){

		std::cout << "\n kinetic energy: " << this->KineticEnergy << " J" << "\n potential energy: " << this->PotentialEnergy << " J";
	}

	std::cout << "\n";
}

void Vehicle::User_set() {
	std::cout << "Input name of the particle: ";
	std::cin >> name;
	std::cout << "Input dry mass (ship without fuel) in kg: ";
	std::cin >> mass;
	std::cout << "Input position x,y,z: ";
	std::cin >> position.x >> position.y >> position.z;
	std::cout << "Input velocity x,y,z in m/s with - to change direction: ";
	std::cin >> velocity.x >> velocity.y >> velocity.z;
	std::cout << "Input fuel mass: ";
	std::cin >> fuel;
	std::cout << "Input fuel usage in kg/s: ";
	std::cin >> fuel_usage;
	std::cout << "\n"; 

	//take fuel mass into account
	mass += fuel;
	
	Print_info();
}

