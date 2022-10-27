#include "Solver.h"
#include <fstream>
#include <filesystem>
#include "../nlohmann/json.hpp"

void Solver::Load_json() {

	std::string json_name;
	std::ifstream file;

	std::cout << "JSON data available: " << std::endl;
	const std::filesystem::path path{ ".\\JSON_files\\" };

	for (auto const& dir_entry : std::filesystem::directory_iterator{ path }) {
		if (dir_entry.is_regular_file() && dir_entry.path().string().ends_with(".json")) {
			std::cout << dir_entry.path().filename().replace_extension() << '\n'; 
		}
	}

	while (1) {
		std::cout << "Enter JSON filename: " << std::endl;
		std::cin >> json_name;
		std::cout << "\n";
		std::string filename = "./JSON_files/";;
		filename.append(json_name + ".json");
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

	using json = nlohmann::json;
	json data = json::parse(file);

	Particle.name = data["ship"]["name"];
	Particle.position.x = data["ship"]["position"][0];
	Particle.position.y = data["ship"]["position"][1];
	Particle.position.z = data["ship"]["position"][2];
	Particle.velocity.x = data["ship"]["velocity"][0];
	Particle.velocity.y = data["ship"]["velocity"][1];
	Particle.velocity.z = data["ship"]["velocity"][2];
	Particle.engine.x = data["ship"]["engine"][0];
	Particle.engine.y = data["ship"]["engine"][1];
	Particle.engine.z = data["ship"]["engine"][2];
	Particle.mass = data["ship"]["mass"];
	Particle.fuel = data["ship"]["fuel"];
	Particle.fuel_usage = data["ship"]["fuel_usage"];

	Push_Back(0);
	std::cout << "\nShip loaded: " << std::endl;
	Particle.Print_info();


	for (auto &p : data["planets"]) {

		Planet planet; 
		planet.name = p["name"];
		planet.mass = p["mass"];
		planet.radius = p["radius"];
		planet.position.x = p["position"][0];
		planet.position.y = p["position"][1];
		planet.position.z = p["position"][2];
		Planets.push_back(planet);
		std::cout << "\nPlanet Added: \n";
		planet.Print_info();
	}

	T = data["data"]["time"];
	t = data["data"]["interval"];
	tx = data["data"]["engines"][0];
	ty = data["data"]["engines"][1];
	tz = data["data"]["engines"][2];

}

void Solver::Check_Collision() {}

void Solver::Populate() {

	double i=0;
	int Planet_Num;
	Planet planet(0, 0, 0, 0, 0, "");

	//Get ammount of planets in sim
	while (std::cout << "Select ammount of planets in simulation: " && !(std::cin >> Planet_Num) || (Planet_Num < 0)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}

	//Set parameters for planets and put in the planets vector
	for (int i = 0; i < Planet_Num; i++) {
		
		std::cout << "Enter name of the planet " << i << " : ";
		std::cin >> planet.name;

		while (std::cout << "Enter mass of the planet " << i << " : " && !(std::cin >> planet.mass) || (planet.mass < 0)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input; please re-enter.\n";
		}
		
		while (std::cout << "Enter radius of the planet " << i << " : " && !(std::cin >> planet.radius) || (planet.radius < 0)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input; please re-enter.\n";
		}

		std::cout << "Enter position x,y,z of the planet " << i << " : ";
		std::cin >> planet.position.x >> planet.position.y >> planet.position.z;

		Planets.push_back(planet);
		std::cout << "\nPlanet Added: \n";
		planet.Print_info();
	}
}

void Solver::Setup() {
	Populate();

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
	Push_Back(0);
}

void Solver::Euler() {

	Vector3D grav_forces;
	Vector3D distance;
	bool engine_used = false;

	for (double time = t; time < T + t; time += t) {

		double fuel_used = Particle.fuel_usage * t;
		grav_forces.Zero();
		distance.Zero();

		if (!Planets.empty()) { 
			for (auto& p : Planets) {

				//Recalculate potential energy
				Particle.PotentialEnergy.Zero();

				//distance between ship and planets
				distance = Particle.position - p.position;

				//Potential energy
				Particle.PotentialEnergy.x -= (G * p.mass * Particle.mass) / (p.radius + distance.x);
				Particle.PotentialEnergy.y -= (G * p.mass * Particle.mass) / (p.radius + distance.y);
				Particle.PotentialEnergy.z -= (G * p.mass * Particle.mass) / (p.radius + distance.z);

				//gravitational forces
				grav_forces.x += ((G * p.mass * Particle.mass) / ((p.radius + distance.x) * (p.radius + distance.x)));
				grav_forces.y += ((G * p.mass * Particle.mass) / ((p.radius + distance.y) * (p.radius + distance.y)));
				grav_forces.z += ((G * p.mass * Particle.mass) / ((p.radius + distance.z) * (p.radius + distance.z)));
			}

			//Change initial value of energy and start printing energy
			if (time == t) {
				Particle.KineticEnergy = { Particle.velocity.x * Particle.mass / 2, Particle.velocity.y * Particle.mass / 2 , Particle.velocity.z * Particle.mass / 2 };
				kinetic_data.front() = Particle.KineticEnergy;
				potential_data.front() = Particle.PotentialEnergy;
				Particle.CalculatedEnergy = true;
			}

		}	

			//check engine forces only if particle has a mass
			if (Particle.mass > 0 ) {

				//will engine be used in this iteration 
				//TODO: not sure if fuel should decrease sepreatly for each plane, or if it is connected so temporaily it will be connected:
				engine_used = false;

				//x
				if (time >= tx && Particle.fuel > 0 && Particle.engine.x != 0) {
					Particle.force.x = Particle.engine.x + grav_forces.x;
					Particle.velocity.x += (Particle.force.x / Particle.mass) * t;
					engine_used = true;
				}

				//y 
				if (time >= ty && Particle.fuel > 0 && Particle.engine.y != 0) {
					Particle.force.y = Particle.engine.y + grav_forces.y;
					Particle.velocity.y += (Particle.force.y / Particle.mass) * t;
					engine_used = true;
				}

				//z
				if (time >= tz && Particle.fuel > 0 && Particle.engine.z != 0) {
					Particle.force.z = Particle.engine.z + grav_forces.z;
					Particle.velocity.z += (Particle.force.z / Particle.mass) * t;
					engine_used = true;
				}

				//remove fuel used 
				if (engine_used) {
					Particle.fuel -= fuel_used;
					Particle.mass -= fuel_used;
				}

				//if engines not used change velocity because of gravitational forces
				else {
					Particle.force = grav_forces;
					Particle.velocity.x += (Particle.force.x / Particle.mass) * t;
					Particle.velocity.y += (Particle.force.y / Particle.mass) * t;
					Particle.velocity.z += (Particle.force.z / Particle.mass) * t;
				}
			}

		//position
		Particle.position += Particle.velocity;

		//displacment
		Particle.displacement += Particle.velocity;

		//kinetic energy 
		Particle.KineticEnergy = { Particle.velocity.x * Particle.mass / 2, Particle.velocity.y * Particle.mass / 2 , Particle.velocity.z * Particle.mass / 2 };

		//save values to vectors
		Push_Back(time);
	}

	//display information at the end of motion:
	std::cout << "\nTime of motion: " << T << " seconds" << std::endl;
	Particle.Print_info();
}

void Solver::Save_data() {

	std::string particle_name = Particle.name;
	std::string filename = "./Simulation_History/Ships/";
	particle_name.append(".txt");
	filename.append(particle_name);
	std::ofstream file;
	file.open(filename.c_str());

	for (auto t = 0; t < time_data.size(); t++) {

		file << time_data[t] << " " << mass_data[t] << " " << fuel_data[t] << " " << Particle.fuel_usage
			<< " " << position_data[t].x << " " << position_data[t].y << " " << position_data[t].z
			<< " " << velocity_data[t].x << " " << velocity_data[t].y << " " << velocity_data[t].z
			<< " " << engine_data[t].x << " " << engine_data[t].y << " " << engine_data[t].z
			<< " " << force_data[t].x << " " << force_data[t].y << " " << force_data[t].z 
			<< " " << kinetic_data[t].x << " " << kinetic_data[t].y << " " << kinetic_data[t].z
			<< " " << potential_data[t].x << " " << potential_data[t].y << " " << potential_data[t].z
			<< std::endl;
	}
	file.close();
}

void Solver::Load_data() {
	
	std::string particle_name;
	std::ifstream file;

	std::cout << "Simulation data available: " << std::endl;
	const std::filesystem::path path{ ".\\Simulation_History\\Ships" };

	//looking for all the files with .txt extenstion in the path:
	for (auto const& dir_entry : std::filesystem::directory_iterator{ path }) {
		if (dir_entry.is_regular_file() && dir_entry.path().string().ends_with(".txt")) {
			std::cout << dir_entry.path().filename().replace_extension() << '\n'; //replace_extenstion removes the .txt from the output
		}
	}

	//opens file from the inputed name:
	while (1) {
		std::cout << "Enter simulation name: " << std::endl;
		std::cin >> particle_name;
		std::cout << "\n";
		std::string filename = "./Simulation_History/Ships/";;
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
	Vector3D position_temp, velocity_temp, engine_temp, force_temp, kin_temp, pot_temp;

	while (file >> time_temp >> mass_temp >> fuel_temp >> fuel_use
			>> position_temp.x >> position_temp.y >> position_temp.z
			>> velocity_temp.x >> velocity_temp.y >> velocity_temp.z
			>> engine_temp.x >> engine_temp.y >> engine_temp.z
			>> force_temp.x >> force_temp.y >> force_temp.z
			>> kin_temp.x >> kin_temp.y >> kin_temp.z
			>> pot_temp.x >> pot_temp.y >> pot_temp.z){

		//load all data from the file to corresponding vectors
		Push_Back(time_temp, mass_temp, fuel_temp, position_temp, velocity_temp, engine_temp, force_temp, kin_temp, pot_temp);
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

		Particle.engine.x = force_data[0].x;
		Particle.engine.y = force_data[0].y;
		Particle.engine.z = force_data[0].z;

		Particle.force.x = force_data[0].x;
		Particle.force.y = force_data[0].y;
		Particle.force.z = force_data[0].z;

		//set displacement to 0
		Particle.displacement.Zero();
		Particle.Print_info();
}

void Solver::Push_Back(double time) {

	time_data.push_back(time);
	mass_data.push_back(Particle.mass);
	fuel_data.push_back(Particle.fuel);
	position_data.push_back(Particle.position);
	velocity_data.push_back(Particle.velocity);
	engine_data.push_back(Particle.engine);
	force_data.push_back(Particle.force);
	kinetic_data.push_back(Particle.KineticEnergy);
	potential_data.push_back(Particle.PotentialEnergy);
}

void Solver::Push_Back(double time, double mass, double fuel, Vector3D position, Vector3D velocity, Vector3D engine, Vector3D force, Vector3D kinetic_energy, Vector3D potential_energy){

	time_data.push_back(time);
	mass_data.push_back(mass);
	fuel_data.push_back(fuel);
	position_data.push_back(position);
	velocity_data.push_back(velocity);
	engine_data.push_back(engine);
	force_data.push_back(force);
	kinetic_data.push_back(kinetic_energy);
	potential_data.push_back(potential_energy);
}

void Solver::Change_data(){

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
	std::cout << "Do you wish to change fuel usage? [y/n]";
	std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Input fuel usage in kg/s: ";
			std::cin >> Particle.fuel_usage;
		}

		//Data changed so recalculate Potential Energy
		Particle.CalculatedEnergy = false;
}

void Solver::Save_planets() {

	std::string particle_name = Particle.name;
	std::string filename = "./Simulation_History/Planets/";
	particle_name.append("_planets.txt");
	filename.append(particle_name);
	std::ofstream file;
	file.open(filename.c_str());

	for (auto& p : Planets) {

		file << p.name << " " << p.mass << " " << p.radius << " " << p.position.x
			<< " " << p.position.y << " " << p.position.z << " " << std::endl;
	}

	file.close();
}

void Solver::Load_planets() {

	std::string particle_name = Particle.name;
	std::string filename = "./Simulation_History/Planets/";
	particle_name.append("_planets.txt");
	filename.append(particle_name);
	std::ifstream file;
	Planet planet(0, 0, 0, 0, 0, "");

	//open file
	file.open(filename.c_str());

	if (!is_empty(file)) {

		//temporary storage for reading from file
		std::string name_temp;
		double mass_temp, radius_temp;
		Vector3D position_temp;

		while (file >> name_temp >> mass_temp >> radius_temp >> 
		position_temp.x >> position_temp.y >> position_temp.z){

			//assign planet's data
			planet.name = name_temp;
			planet.mass = mass_temp;
			planet.radius = radius_temp;
			planet.position = position_temp;

			//load planets to solver vector Planets
			Planets.push_back(planet);
			std::cout << "\nPlanet Loaded: \n";
			planet.Print_info();
		}
	}
	file.close();
}

void Solver::Change_planets() {

	enum choice { Add, Edit, Remove };
	int option = -1;
	int i = 0;
	//first ask if he wants to change or remove any of existing planets
	for (auto& p : Planets) {
		std::cout << "\nCurrent Planet: " << std::endl;
		p.Print_info();

		while (std::cout << "\nPlease choose if you want to: \n 0.Add new planet \n 1.Change current planet \n 2.Remove current planet " && !(std::cin >> option) || (option < 0 || option > 2)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid Selection\n";
		}

		switch (option) {
		case Add:{
			Planet planet(true);
			Planets.push_back(planet);
			std::cout << "\nPlanet Added: \n";
			planet.Print_info();
			break;
		}
		case Edit: {
			p.Change_info();
			break; }

		case Remove: {   
			Planets.erase(Planets.begin() + i);
			if (Planets.size() == 1) {
				Planets.clear();
			}

			break; }

		default: {
			std::cout << "Invalid Selection\n";
			break;
		}
		}
		if (Planets.empty()) break;
		std::cout << "\nCurrent Planet after changes: " << std::endl;
		p.Print_info();
		i++;
	} 

}

bool Solver::is_empty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}