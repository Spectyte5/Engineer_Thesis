#include "Vehicle.h"

Point_Particle::Point_Particle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double ex, double ey, double ez, double m, double f, double u) {

	//name 
	name = n;
	
	//position
	position = { rx, ry, rz };

	//velocities
	velocity = { vx, vy, vz };
	
	//forces
	engine = { ex, ey, ez };
	
	//mass
	mass = m;
	
	//fuel
	fuel = f;
	fuel_usage = u;

	//Energy
	KineticEnergy = { velocity.x * mass / 2, velocity.y * mass / 2 , velocity.z * mass / 2 };
}

void Point_Particle::Print_info() {
	
	std::cout << this->name << " Data: " << "\n mass: " << this->mass << " kg" << "\n position : " 
		<< this->position << " m" << "\n displacement : " << this->displacement << " m" << "\n velocity : "
		<< this->velocity << " m/s" << "\n engine force : " << this->engine << " N" << "\n net force : " 
		<< this->force << " N" << "\n fuel mass : " << this->fuel << " kg" << "\n fuel consumption : " << this->fuel_usage << " kg/s";

	//check if potential energy can be displaced (Are planets initialzed?)	
	if (CalculatedEnergy){

		std::cout << "\n kinetic energy: " << this->KineticEnergy << " J" << "\n potential energy: " << this->PotentialEnergy << " J";
	}

	std::cout << "\n";
}

void Point_Particle::User_set() {
	std::cout << "Input name of the particle: ";
	std::cin >> name;
	std::cout << "Input mass in kg: ";
	std::cin >> mass;
	std::cout << "Input position x,y,z: ";
	std::cin >> position.x >> position.y >> position.z;
	std::cout << "Input velocity x,y,z in m/s with - to change direction: ";
	std::cin >> velocity.x >> velocity.y >> velocity.z;
	std::cout << "Input engine force x,y,z in N with - to change direction: ";
	std::cin >> engine.x >> engine.y >> engine.z;
	std::cout << "Input fuel mass: ";
	std::cin >> fuel;
	std::cout << "Input fuel usage in kg/s: ";
	std::cin >> fuel_usage;
	std::cout << "\n"; 

	//take fuel mass into account
	mass += fuel;
	
	Print_info();
}

