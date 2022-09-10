#include "Solver.h"
#include <fstream>

void Solver::Euler(Point_Particle& Particle) {

	while (std::cout << "Input a time of motion: " && !(std::cin >> T) || (T < 0)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}
	while (std::cout << "Input a time step size (Time between measurments): " && !(std::cin >> t) || (t < 0 || t > T)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}
	while (std::cout << "Input time of turning on the engines (x y z): " && !(std::cin >> tx >> ty >> tz) || (tx < 0 && ty < 0 && tz < 0) || (tx > T && ty > T && tz > T)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}

	for (double time = 0; time < T; time += t) {

		//velocity
		if (Particle.mass > 0) {

			//x
			if (time >= tx && Particle.fuel > 0 && Particle.force.x != 0) {
				Particle.velocity.x += (Particle.force.x / Particle.mass) * t;
				Particle.fuel -= Particle.fuel_usage * t; //fuel used
				Particle.mass -= Particle.fuel_usage * t;
			}

			//y
			if (time >= ty && Particle.fuel > 0 && Particle.force.y != 0) {
				Particle.velocity.y += (Particle.force.y / Particle.mass) * t;
				Particle.fuel -= Particle.fuel_usage * t;
				Particle.mass -= Particle.fuel_usage * t;
			}

			//z
			if (time >= tz && Particle.fuel > 0 && Particle.force.z != 0) {
				Particle.velocity.z += (Particle.force.z / Particle.mass) * t;
				Particle.fuel -= Particle.fuel_usage * t;
				Particle.mass -= Particle.fuel_usage * t;
			}
		}

		//position
			//x
		Particle.position.x += Particle.velocity.x;
		//y
		Particle.position.y += Particle.velocity.y;
		//z
		Particle.position.z += Particle.velocity.z;

		//displacment
			//x
		Particle.displacement.x += Particle.velocity.x;
		//y
		Particle.displacement.y += Particle.velocity.y;
		//z
		Particle.displacement.z += Particle.velocity.z;

		time_data.push_back(time);
		mass_data.push_back(Particle.mass);
		fuel_data.push_back(Particle.fuel);
		position_data.push_back(Particle.position);
		velocity_data.push_back(Particle.velocity);
		force_data.push_back(Particle.force);

	}

	std::cout << "\nTime of motion: " << T << " seconds" << std::endl;
	Particle.Print_info();
}

void Solver::Save_data(Point_Particle& Particle) {

	std::string particle_name = Particle.name;
	std::string filename = "./Simulation_History/";
	particle_name.append(".txt");
	filename.append(particle_name);
	std::ofstream file;
	file.open(filename.c_str());

	for (auto t = 0; t < time_data.size(); t++) {

		file << time_data[t] << " " << mass_data[t] << " " << fuel_data[t]
			<< " " << position_data[t].x << " " << position_data[t].z << " " << position_data[t].y
			<< " " << velocity_data[t].x << " " << velocity_data[t].z << " " << velocity_data[t].y
			<< " " << force_data[t].x << " " << force_data[t].z << " " << force_data[t].y << std::endl;
	}
	file.close();
}