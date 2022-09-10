#include "Vehicle.h"

Point_Particle::Point_Particle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double fx, double fy, double fz, double m, double f, double u) {

	//name 
	name = n;
	//position
	position.x = rx;
	position.y = ry;
	position.z = rz;
	//velocities
	velocity.x = vx;
	velocity.y = vy;
	velocity.z = vz;
	//forces
	force.x = fx;
	force.y = fy;
	force.z = fz;
	//mass
	mass = m;
	//fuel
	fuel = f;
	fuel_usage = u;
}

void Point_Particle::Print_info() {
	
	std::cout << this->name << " Data: " << "\n mass: " << this->mass << " kg" <<"\n position : " 
		<< this->position << "\n displacement : " << this->displacement << " m" << "\n velocity : " 
		<< this->velocity << " m/s" << "\n force : " << this->force << " N" << "\n fuel mass : " 
		<< this->fuel << " kg" << "\n fuel consumption : " << this->fuel_usage << " kg/s" << std::endl;
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
	std::cout << "Input force x,y,z in N with - to change direction: ";
	std::cin >> force.x >> force.y >> force.z;
	std::cout << "Input fuel mass: ";
	std::cin >> fuel;
	std::cout << "Input fuel usage in kg/s: ";
	std::cin >> fuel_usage;
	std::cout << "\n"; 

	//take fuel mass into account
	mass += fuel;
	
	Print_info();
}

