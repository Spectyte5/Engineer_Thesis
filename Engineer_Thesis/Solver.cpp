#include "Solver.h"
#include <filesystem>
#include "../nlohmann/json.hpp"

void Solver::Write_initial() {

	if (!TimeVect.empty()) {
		if (TimeVect.front().timestart.x == 0) {
			Particle.engine.x = TimeVect.front().engforce.x;
		}
		if (TimeVect.front().timestart.y == 0) {
			Particle.engine.y = TimeVect.front().engforce.y;
		}
		if (TimeVect.front().timestart.z == 0) {
			Particle.engine.z = TimeVect.front().engforce.z;
		}
	}
	Push_Back(0);
}

void Solver::Populate() {

	double i=0;
	short int Planet_Num=0;
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

	Particle.User_set(); //setup particle
	Populate(); //setup planets
	short int interval_num=0;
	EngineTimes interval;

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

	std::cout << "\nEngine Control Section: " << std::endl;

	while (std::cout << "How many time intervals will be used for engine control?" && !(std::cin >> interval_num) || (interval_num < 0)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}

	//fill all intervals until input is correct
	for (int i = 0; i < interval_num && interval.Check_input(); i++) {

		std::cout << "Interval  " << i << ": Enter time of turning on the engines (x y z): " << std::endl;
		std::cin >> interval.timestart.x >> interval.timestart.y >> interval.timestart.z;

		std::cout << "Interval  " << i << ": Enter time of turning off the engines (x y z): " << std::endl;
		std::cin >> interval.timeend.x >> interval.timeend.y >> interval.timeend.z;

		std::cout << "Interval  " << i << ": Enter engine force at this interval (x y z): " << std::endl;
		std::cin >> interval.engforce.x >> interval.engforce.y >> interval.engforce.z;

		if (interval.Check_input()) {
				TimeVect.push_back(interval);
				interval.Print_Interval();
		}
		else {
			TimeVect.push_back(interval);  //TODO: fix this check input, because for now it does not work
			interval.Print_Interval();
		}
	}

	//write initial values to vectors
	Write_initial();
}

void Solver::Save_json() {

	using json = nlohmann::json;

	// create an empty structure
	json data;
	data["ship"]["name"] = Particle.name;
	data["ship"]["position"][0] = Particle.position.x;
	data["ship"]["position"][1] = Particle.position.y;
	data["ship"]["position"][2] = Particle.position.z;
	data["ship"]["velocity"][0] = Particle.velocity.x;
	data["ship"]["velocity"][1] = Particle.velocity.y;
	data["ship"]["velocity"][2] = Particle.velocity.z;
	data["ship"]["mass"] = Particle.mass;
	data["ship"]["fuel"] = Particle.fuel;
	data["ship"]["fuel_usage"] = Particle.fuel_usage;

	for (auto& p : Planets) {
		auto i = &p - &Planets[0];
		data["planets"][i]["name"] = p.name;
		data["planets"][i]["mass"] = p.mass;
		data["planets"][i]["radius"] = p.radius;
		data["planets"][i]["position"][0] = p.position.x;
		data["planets"][i]["position"][1] = p.position.y;
		data["planets"][i]["position"][2] = p.position.z;
	}
	for (auto& t : TimeVect) {
		auto i = &t - &TimeVect[0];
		data["control"][i]["starttime"][0] = t.timestart.x;
		data["control"][i]["starttime"][1] = t.timestart.y;
		data["control"][i]["starttime"][2] = t.timestart.z;
		data["control"][i]["endtime"][0] = t.timeend.x;
		data["control"][i]["endtime"][1] = t.timeend.y;
		data["control"][i]["endtime"][2] = t.timeend.z;
		data["control"][i]["force"][0] = t.engforce.x;
		data["control"][i]["force"][1] = t.engforce.y;
		data["control"][i]["force"][2] = t.engforce.z;
	}

	data["data"]["time"] = T;
	data["data"]["step"] = t;

	std::string path = "./JSON_files/" + Particle.name + ".json";
	std::ofstream file(path.c_str());
	file << std::setw(4) << data << std::endl;
	std::cout << "File saved Successfully as: " << path << std::endl;
}

std::ifstream Solver::Load_file(std::string sys_path, std::string filepath, std::string extenstion) {

	std::ifstream file;
	std::string file_name;

	std::cout << "Data available: " << std::endl;
	const std::filesystem::path path{ sys_path.c_str() };

	for (auto const& dir_entry : std::filesystem::directory_iterator{ path }) {
		if (dir_entry.is_regular_file() && dir_entry.path().string().ends_with(extenstion.c_str())) {
			std::cout << dir_entry.path().filename().replace_extension() << '\n';
		}
	}

	while (1) {
		std::cout << "Enter Name of your file: " << std::endl;
		std::cin >> file_name;
		std::cout << "\n";
		std::string filename = filepath;;
		filename.append(file_name + extenstion.c_str());
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
	return file;
}

void Solver::Load_data(std::string& filename) {

	std::ifstream file;

	if (filename != "") {
		file.open(filename);
	}

	else {
		file = Load_file(".\\JSON_files\\", "./JSON_files/", ".json");
	}

	using json = nlohmann::json;
	json data = json::parse(file);

	Particle.name = data["ship"]["name"];
	Particle.position = { data["ship"]["position"][0], data["ship"]["position"][1], data["ship"]["position"][2] };
	Particle.velocity = { data["ship"]["velocity"][0], data["ship"]["velocity"][1], data["ship"]["velocity"][2] };
	Particle.mass = data["ship"]["mass"];
	Particle.fuel = data["ship"]["fuel"];
	Particle.fuel_usage = data["ship"]["fuel_usage"];

	std::cout << "\nShip loaded: " << std::endl;
	Particle.Print_info();


	for (auto& p : data["planets"]) {

		Planet planet;
		planet.name = p["name"];
		planet.mass = p["mass"];
		planet.radius = p["radius"];
		planet.position = { p["position"][0], p["position"][1], p["position"][2] };
		Planets.push_back(planet);
		std::cout << "\nPlanet Added: \n";
		planet.Print_info();
	}

	for (auto& c : data["control"]) {

		EngineTimes interval;
		//engine interval start
		interval.timestart = { c["starttime"][0], c["starttime"][1], c["starttime"][2] };
		//engine interval end
		interval.timeend = { c["endtime"][0], c["endtime"][1], c["endtime"][2] };
		//engine interval force 
		interval.engforce = { c["force"][0],c["force"][1], c["force"][2] };
		//save time interval to vector
		TimeVect.push_back(interval);
		//print time interval
		std::cout << "\nTime interval Added: \n";
		interval.Print_Interval();
	}

	Write_initial();

	T = data["data"]["time"];
	t = data["data"]["step"];

	file.close();
}

bool Solver::Check_Collision(Planet& Planet) {

	Vector3D Distances = { pow((Particle.position.x - Planet.position.x), 2),
		pow((Particle.position.y - Planet.position.y), 2),
		pow((Particle.position.z - Planet.position.z), 2) };

	// distance between the centre and given point
	double distance = Distances.x + Distances.y + Distances.z;

	if (distance < (Planet.radius * Planet.radius)) {
		std::cout << "\nShip: " << Particle.name << ", has crashed into Planet: " << Planet.name
			<< " !\n********************************************************" << std::endl;
		return true;
	}

	// distance btw centre and point is equal to radius
	else if (distance == (Planet.radius * Planet.radius)) {

		//if it is on surface it can be a start from the planet and a try to escape the orbit, so i return it as not a full collistion, 
		std::cout << "\nShip: " << Particle.name << ", is located on Planet: " << " Surface" << Planet.name << "!" << std::endl;
		return false;
	}

	// distance btw center and point is greater than radius
	else return false;
}

bool Solver::UseEngine(double& time) {

	bool used = false;
	//TODO: Check fuel

	for (auto& i : TimeVect) {

		used = false;
		//check if simulation step is contained in one of the intervals
		//x
		if (time >= i.timestart.x && time <= i.timeend.x) {
			Particle.engine.x = i.engforce.x; //if yes turn on the engine
			used = true;
		}

		//y
		if (time >= i.timestart.y && time <= i.timeend.y) {
			Particle.engine.y = i.engforce.y;
			used = true;
		}

		//z
		if (time >= i.timestart.z && time <= i.timeend.z) {
			Particle.engine.z = i.engforce.z;
			used = true;
		}

		//because current time can only be in one interval we do not need to check all of them
		if (used) {
			return true;
		}
	}
	return false;
}


void Solver::Euler() {

	Vector3D grav_forces;
	Vector3D distance;
	bool engine_used = false;

	for (double time = t; time < T + t; time += t) {

		double fuel_used = Particle.fuel_usage * t;
		grav_forces.Zero();
		distance.Zero();
		Particle.PotentialEnergy.Zero();

		if (!Planets.empty()) { 
			for (auto& p : Planets) {

				//check if ship collides with the planet
				if (Check_Collision(p)) {
					exit(0); //stop simulation
				}

				//distance between ship and planets
				distance = { abs(Particle.position.x - p.position.x), abs(Particle.position.y - p.position.y), abs(Particle.position.z - p.position.z) };

				//Potential energy
				Particle.PotentialEnergy -= {(G* p.mass* Particle.mass) / (distance.x), (G* p.mass* Particle.mass) / (distance.y), (G* p.mass* Particle.mass) / (distance.z)};

				//gravitational forces
				grav_forces += {((G * p.mass * Particle.mass) / (distance.x * distance.x)), ((G * p.mass * Particle.mass) / (distance.y * distance.y)), ((G * p.mass * Particle.mass) / (distance.z * distance.z))
			};
				
			}

			//Change initial value of energy and start printing energy
			if (time == t) {
				Particle.KineticEnergy = { Particle.velocity.x * Particle.mass / 2, Particle.velocity.y * Particle.mass / 2 , Particle.velocity.z * Particle.mass / 2 };
				kinetic_data.front() = Particle.KineticEnergy;
				potential_data.front() = Particle.PotentialEnergy;
				Particle.CalculatedEnergy = true;
			}

		}	
			//check forces only if particle has a mass 
		if (Particle.mass > 0) {

			//will engine be used in this iteration 
			engine_used = false;
			//engine is used only if time intervals vector is not empty
			if (!TimeVect.empty()) {
				engine_used = UseEngine(time);
			}
			//remove fuel used 
			if (engine_used) {
				Particle.fuel -= fuel_used;
				Particle.mass -= fuel_used;
			}
			//if engine not used
			else {
				Particle.engine.Zero();
			}
			//calculate velocites
			Particle.force = { Particle.engine.x + grav_forces.x , grav_forces.y + Particle.engine.y, grav_forces.z + Particle.engine.z };
			Particle.velocity += {(Particle.force.x / Particle.mass) * t, (Particle.force.y / Particle.mass) * t, (Particle.force.z / Particle.mass) * t };
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

void Solver::Save_data() {

	std::string particle_name = Particle.name;
	std::string filename = "./Simulation_History/Ships/";
	particle_name.append(".txt");
	filename.append(particle_name);
	std::ofstream file;
	file.open(filename.c_str());
	file.precision(10);
	
	//write informational line
	file << "Time[s] " << "Mass[kg] " << "Fuel[kg]" << " FuelUsage[kg/s]" 
		<< " Posx(t)[m]" << " Posy(t)[m]" << " Posz(t)[m]" 
		<< " Velx(t)[m/s]" << " Vely(t)[m/s]" << " Velz(t)[m/s]"
		<< " EngineFx[N]" << " EngineFy[N]" << " EngineFz[N]"
		<< " NetFx[N]" << " NetFy[N]" << " NetFz[N]"
		<< " EKinx[J]" << " EKiny[J]" << " EKinz[J]"
		<< " EPotx[J]" << " EPoty[J]" << " EPotz[J]" << std::endl;

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

	Save_planets();
}

void Solver::Save_planets() {

	std::string particle_name = Particle.name;
	std::string filename = "./Simulation_History/Planets/";
	particle_name.append("_planets.txt");
	filename.append(particle_name);
	std::ofstream file;
	file.open(filename.c_str());
	file.precision(10);

	//write informational line
	file << "Name " << "Mass[kg] " << "Radius[m]"
		<< " Posx(t)[m]" << " Posy(t)[m]" << " Posz(t)[m]" << std::endl;


	for (auto& p : Planets) {

		file << p.name << " " << p.mass << " " << p.radius << " " << p.position.x
			<< " " << p.position.y << " " << p.position.z << " " << std::endl;
	}

	file.close();
}

