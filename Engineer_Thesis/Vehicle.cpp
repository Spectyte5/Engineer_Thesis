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

void Point_Particle::Print_info() {
	
	std::cout << this->name << " Data: " << "\n mass: " << this->mass << " kg" <<"\n position : " << this->position << "\n displacement : " << this->displacement << " m" << "\n velocity : " << this->velocity << " m/s" << "\n force : " << this->force << " N" << std::endl;
}

void Point_Particle::User_set() {
	std::cout << "Input name of the particle: ";
	std::cin >> name;
	std::cout << "Input mass in kg: ";
	std::cin >> mass;
	std::cout << "Input position x,y,z: ";
	std::cin >> position.x >> position.y >> position.z;
	std::cout << "Input velocity x,y,z in m/s: ";
	std::cin >> velocity.x >> velocity.y >> velocity.z;
	std::cout << "Input force x,y,z:";
	std::cin >> force.x >> force.y >> force.z;
	std::cout << "\n"; 

	Print_info();
}

void Point_Particle::Euler() {
	
	//T final time,max time 100s (just for now)
	double T=0, Tmax = 100;

	while (std::cout << "Input a time of motion, not longer than " << Tmax << " s: " && !(std::cin >> T) || (T < 0 || T > Tmax)) {
		std::cin.clear(); 
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		std::cout << "Invalid input; please re-enter.\n";
	}
	
	for (int t = 0; t < T; t += 1) {	
	
	//velocity
		if (mass > 0) {
			//x
			velocity.x += force.x / mass;
			//y
			velocity.y += force.y / mass;
			//z
			velocity.z += force.z / mass;
		}
	//position
		//x
		position.x += velocity.x;
		//y
		position.y += velocity.y;
		//z
		position.z += velocity.z;
	//displacment
				//x
		displacement.x += velocity.x;
		//y
		displacement.y += velocity.y;
		//z
		displacement.z += velocity.z;
	}

	std::cout << "\nTime of motion: " << T << " seconds" << std::endl;
	Print_info();
}