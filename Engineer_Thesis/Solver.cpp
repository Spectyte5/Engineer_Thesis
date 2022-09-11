#include "Solver.h"
#include <fstream>
#include <filesystem>

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
	while (std::cout << "Input time of turning on the engines (x y z): " && !(std::cin >> tx >> ty >> tz) || (tx < 0 || ty < 0 || tz < 0) || (tx > T || ty > T || tz > T)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}

	//write initial values to vectors
	Push_Back(Particle, 0);

	for (double time = t; time < T+t; time += t) {

		double fuel_used = Particle.fuel_usage * t;

		//velocity
		if (Particle.mass > 0) {

			//x
			if (time >= tx && Particle.fuel > 0 && Particle.force.x != 0) {
				Particle.velocity.x += (Particle.force.x / Particle.mass) * t;
				Particle.fuel -= fuel_used; //fuel used
				Particle.mass -= fuel_used;
			}

			//y
			if (time >= ty && Particle.fuel > 0 && Particle.force.y != 0) {
				Particle.velocity.y += (Particle.force.y / Particle.mass) * t;
				Particle.fuel -= fuel_used;
				Particle.mass -= fuel_used;
			}

			//z
			if (time >= tz && Particle.fuel > 0 && Particle.force.z != 0) {
				Particle.velocity.z += (Particle.force.z / Particle.mass) * t;
				Particle.fuel -= fuel_used;
				Particle.mass -= fuel_used;
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

		//save values to vectors
		Push_Back(Particle, time);
	}

	//display information at the end of motion:
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

		file << time_data[t] << " " << mass_data[t] << " " << fuel_data[t] << " " << Particle.fuel_usage
			<< " " << position_data[t].x << " " << position_data[t].z << " " << position_data[t].y
			<< " " << velocity_data[t].x << " " << velocity_data[t].z << " " << velocity_data[t].y
			<< " " << force_data[t].x << " " << force_data[t].z << " " << force_data[t].y << std::endl;
	}
	file.close();
}

void Solver::Load_data(Point_Particle& Particle) {
	
	std::string particle_name;
	std::ifstream file;
	int i = 0;

	std::cout << "Simulation data available: " << std::endl;
	const std::filesystem::path path{ ".\\Simulation_History" };

	//looking for all the files with .txt extenstion in the path:
	for (auto const& dir_entry : std::filesystem::directory_iterator{ path }) {
		if (dir_entry.is_regular_file() && dir_entry.path().string().ends_with(".txt")) {
			std::cout << i << ". " << dir_entry.path().filename().replace_extension() << '\n'; //replace_extenstion removes the .txt from the output
			i++;
		}
	}

	//opens file from the inputed name:
	while (1) {
		std::cout << "Enter simulation name: " << std::endl;
		std::cin >> particle_name;
		std::cout << "\n";
		std::string filename = "./Simulation_History/";;
		filename.append(particle_name + ".txt");
		//check if file exists:
		file.open(filename.c_str());
		if (!file) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			filename.clear();
			std::cout << "\nFile not found\n" << std::endl;
		}
		else {
			break;
		}
	}

	
	//temporary storage for reading from file
	double time_temp, mass_temp, fuel_temp, fuel_use;
	Vector3D position_temp, velocity_temp, force_temp;

	while (!file.eof()) {
	
		file >> time_temp >> mass_temp >> fuel_temp >> fuel_use
			>> position_temp.x >> position_temp.z >> position_temp.y
			>> velocity_temp.x >> velocity_temp.z >> velocity_temp.y
			>> force_temp.x >> force_temp.z >> force_temp.y;

		//load all data from the file to corresponding vectors
		Push_Back(time_temp, mass_temp, fuel_temp, position_temp, velocity_temp, force_temp);
	}

		//set particle to initial value if user wants to run the same simulation again
		Particle.fuel_usage = fuel_use;
		Particle.name = particle_name;
		Particle.mass = mass_data[0];
		Particle.fuel = fuel_data[0];

		Particle.position.x = position_data[0].x;
		Particle.position.y = position_data[0].y;
		Particle.position.z = position_data[0].z;

		Particle.velocity.x = velocity_data[0].x;
		Particle.velocity.y = velocity_data[0].y;
		Particle.velocity.z = velocity_data[0].z;

		Particle.force.x = force_data[0].x;
		Particle.force.y = force_data[0].y;
		Particle.force.z = force_data[0].z;

		//calculate displacement
		Particle.displacement.x = position_data.back().x - position_data.front().x;
		Particle.displacement.y = position_data.back().y - position_data.front().y;
		Particle.displacement.z = position_data.back().z - position_data.front().z;


		Particle.Print_info();
}

void Solver::Push_Back(Point_Particle& Particle, double time) {

	time_data.push_back(time);
	mass_data.push_back(Particle.mass);
	fuel_data.push_back(Particle.fuel);
	position_data.push_back(Particle.position);
	velocity_data.push_back(Particle.velocity);
	force_data.push_back(Particle.force);
}

void Solver::Push_Back(double time, double mass, double fuel, Vector3D position, Vector3D velocity, Vector3D force) {

	time_data.push_back(time);
	mass_data.push_back(mass);
	fuel_data.push_back(fuel);
	position_data.push_back(position);
	velocity_data.push_back(velocity);
	force_data.push_back(force);
}

void Solver::Change_data(Point_Particle& Particle){

	char choice = 'n';
	std::cout << "Data changing of the particle ";
	std::cout << "Do you wish to change name? [y/n]";
	std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Input name of the particle: ";
			std::cin >> Particle.name;
		}
	std::cout << "Do you wish to change mass? [y/n]";
	std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Input mass in kg: ";
			std::cin >> Particle.mass;
			std::cout << "Input fuel mass: ";
			std::cin >> Particle.fuel;
			Particle.mass += Particle.fuel;
		}
	std::cout << "Do you wish to change position? [y/n]";
	std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Input position x,y,z: ";
			std::cin >> Particle.position.x >> Particle.position.y >> Particle.position.z;
		}
	std::cout << "Do you wish to change velocity? [y/n]";
	std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Input velocity x,y,z in m/s with - to change direction: ";
			std::cin >> Particle.velocity.x >> Particle.velocity.y >> Particle.velocity.z;
		}
	std::cout << "Do you wish to change force? [y/n]";
	std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Input force x,y,z in N with - to change direction: [y/n]";
			std::cin >> Particle.force.x >> Particle.force.y >> Particle.force.z;
		}
	std::cout << "Do you wish to change name? [y/n]";
	std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Input fuel usage in kg/s: ";
			std::cin >> Particle.fuel_usage;
		}
}