#include "Vehicle.h"

Point_Particle::Point_Particle(std::string n, double rx, double ry, double rz, double vx, double vy, double vz, double fx, double fy, double fz, double m) {

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
}

//Print information about a Particle
void Point_Particle::Print_info() {
	
	std::cout << this->name << " Data: " << "\n mass: " << this->mass << " kg" <<"\n position : " << this->position << "\n velocity : " << this->velocity << " m/s" << "\n force : " << this->force << " N" << std::endl;
}