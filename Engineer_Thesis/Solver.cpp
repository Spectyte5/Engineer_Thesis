#include "Solver.h"
#include <filesystem>
#include "../nlohmann/json.hpp"

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
	Print_Pauses();
	Particle.User_set(); //setup particle
	Print_Pauses();
	Populate(); //setup planets
	Print_Pauses();
	short int interval_num=0;
	Control interval;

	while (std::cout << "Input a time of motion: " && !(std::cin >> T) || (T < 0)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}
	while (std::cout << "Input a time step size (Time between measurments): " && !(std::cin >> step) || (step < 0 || step > T)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}

	while (std::cout << "Which Method should be used for solving ODE: \n 0.Adam's Bashforth \n 1.Euler \n 2.Midpoint \n 3.Runge Kutta IV: " && !(std::cin >> method) || (method < 0 || method > 3)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid Selection\n";
	}

	Print_Pauses();

	std::cout << "Engine Control Section: " << std::endl;

	while (std::cout << "How many time intervals will be used for engine control?" && !(std::cin >> interval_num) || (interval_num < 0)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input; please re-enter.\n";
	}

	//fill all intervals until input is correct
	for (int i = 0; i < interval_num ; i++) {

		std::cout << "Interval " << i << ": Enter time of turning on the engines (x y z): " << std::endl;
		std::cin >> interval.timestart.x >> interval.timestart.y >> interval.timestart.z;

		std::cout << "Interval " << i << ": Enter time of turning off the engines (x y z): " << std::endl;
		std::cin >> interval.timeend.x >> interval.timeend.y >> interval.timeend.z;

		std::cout << "Interval " << i << ": Enter engine force at this interval (x y z): " << std::endl;
		std::cin >> interval.engforce.x >> interval.engforce.y >> interval.engforce.z;

		if (interval.Check_input(*this)) {
				TimeVect.push_back(interval);
				interval.Print_Interval();
		}
		else { i--; }
	}
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
	for (auto& step : TimeVect) {
		auto i = &step - &TimeVect[0];
		data["control"][i]["starttime"][0] = step.timestart.x;
		data["control"][i]["starttime"][1] = step.timestart.y;
		data["control"][i]["starttime"][2] = step.timestart.z;
		data["control"][i]["endtime"][0] = step.timeend.x;
		data["control"][i]["endtime"][1] = step.timeend.y;
		data["control"][i]["endtime"][2] = step.timeend.z;
		data["control"][i]["force"][0] = step.engforce.x;
		data["control"][i]["force"][1] = step.engforce.y;
		data["control"][i]["force"][2] = step.engforce.z;
	}

	data["data"]["time"] = T;
	data["data"]["step"] = step;
	data["data"]["ode"] = method;

	Print_Pauses();
	std::string path = "./JSON_files/" + Particle.name + ".json";
	std::ofstream file(path.c_str());
	file << std::setw(4) << data << std::endl;
	std::cout << "File saved Successfully as: " << path << std::endl;
	Print_Pauses();
}

std::ifstream Solver::Load_file(std::string sys_path, std::string filepath, std::string extenstion) {

	std::ifstream file;
	std::string file_name;

	Print_Pauses();
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
	Print_Pauses();
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
	
	std::cout << "Ship:\n\n" << "Ship loaded: " << std::endl;
	Particle.Print_info();
	Print_Pauses();

	std::cout << "Planets: \n";
	for (auto& p : data["planets"]) {

		Planet planet;
		planet.name = p["name"];
		planet.mass = p["mass"];
		planet.radius = p["radius"];
		planet.position = { p["position"][0], p["position"][1], p["position"][2] };
		Planets.push_back(planet);
		std::cout << "\nPlanet Added:";
		planet.Print_info();
	}
	Print_Pauses();
	std::cout << "Intervals: \n";
	for (auto& c : data["control"]) {

		Control interval;
		//engine interval start
		interval.timestart = { c["starttime"][0], c["starttime"][1], c["starttime"][2] };
		//engine interval end
		interval.timeend = { c["endtime"][0], c["endtime"][1], c["endtime"][2] };
		//engine interval force 
		interval.engforce = { c["force"][0],c["force"][1], c["force"][2] };
		//save time interval to vector
		TimeVect.push_back(interval);
		//print time interval
		std::cout << " \nTime interval Added:";
		interval.Print_Interval();
	}
	Print_Pauses();

	T = data["data"]["time"];
	step = data["data"]["step"];
	method = data["data"]["ode"];
	std::cout << "Times:\n\nTime loaded: " << T << std::endl;
	std::cout << "Step loaded: " << step << std::endl;

	file.close();
	Print_Pauses();
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
		
		//first check if there is fuel:
		if (Particle.fuel <= 0) {
			std::cout << "\nRun Out of fuel, engines won't be used from now on..." << std::endl;
			TimeVect.clear(); //if there is no fuel there is no reason to check engine usage
			break;
		}

		//check if simulation step is contained in one of the intervals
		//x
		if (time >= i.timestart.x && time <= i.timeend.x ) {
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

void Solver::Calculate_Net() {

	//will engine be used in this iteration 
	engine_used = false;
	//engine is used only if time intervals vector is not empty
	if (!TimeVect.empty()) {
		engine_used = UseEngine(time);
	}
	//remove fuel used 
	if (engine_used) {
		fuel_used = Particle.fuel_usage * step; //calculate fuel used
		Particle.fuel -= fuel_used;
		Particle.mass -= fuel_used;
	}
	//if engine not used
	else {
		Particle.engine.Zero();
	}
	//calculate net force
	Particle.force = { Particle.engine.x + grav_forces.x , grav_forces.y + Particle.engine.y, grav_forces.z + Particle.engine.z };
}

void Solver::Calculate_Grav() {

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
		grav_forces += {((G* p.mass* Particle.mass) / (distance.x * distance.x)), ((G* p.mass* Particle.mass) / (distance.y * distance.y)), ((G* p.mass* Particle.mass) / (distance.z * distance.z))
		};

	}

	//Change initial value of energy and start printing energy
	if (time == step) {
		Particle.KineticEnergy = { Particle.velocity.x * Particle.mass / 2, Particle.velocity.y * Particle.mass / 2 , Particle.velocity.z * Particle.mass / 2 };
		kinetic_data.front() = Particle.KineticEnergy;
		potential_data.front() = Particle.PotentialEnergy;
		Particle.CalculatedEnergy = true;
	}

}

void Solver::Euler(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {

	//Particle.velocity += {(Particle.force.x / Particle.mass)* step, (Particle.force.y / Particle.mass)* step, (Particle.force.z / Particle.mass)* step };
	//Particle.position += Particle.velocity;
	velocity += dvdt(time + dt, velocity,force,mass);
	position += dxdt(time + dt , velocity, position);
}

//runge kutta method
void Solver::Runge_Kutta(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {

	double  k1, k2, k3, k4, h1, h2, h3, h4;

	k1 = dvdt(time, velocity, force, mass);
	h1 = dxdt(time, velocity, position);
	k2 = dvdt(time + dt / 2., velocity + k1 / 2., force, mass);
	h2 = dxdt(time + dt / 2., velocity + k1 / 2., position + h1 / 2.);
	k3 = dvdt(time + dt / 2., velocity + k2 / 2., force, mass);
	h3 = dxdt(time + dt / 2., velocity + k2 / 2., position + h2 / 2.);
	k4 = dvdt(time + dt, velocity + k3, force, mass);
	h4 = dxdt(time + dt, velocity + k3, position + h3);

	velocity += 1.0 / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);
	position += 1.0 / 6.0 * (h1 + 2 * h2 + 2 * h3 + h4);
}

void Solver::Midpoint(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {

	double mx,mv;

	mv = dvdt(time, velocity, force, mass);
	mx = dxdt(time, velocity, position);
	velocity += dvdt(time + dt / 2., velocity + mv / 2., force, mass);
	position += dxdt(time + dt / 2., velocity + mv / 2., position + mx / 2.);
}

//Adam - Bashforth method
void Solver::Adams_Bashford(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {

	double  k0, k1, k2, k3, h0, h1, h2, h3;

	//Calculate initial steps RK4
		k1 = dvdt(time, velocity, force, mass);
		h1 = dxdt(time, velocity, position);
		k2 = dvdt(time + dt / 2., velocity + k1 / 2., force, mass);
		h2 = dxdt(time + dt / 2., velocity + k1 / 2., position + h1 / 2.);
		k3 = dvdt(time + dt / 2., velocity + k2 / 2., force, mass);
		h3 = dxdt(time + dt / 2., velocity + k2 / 2., position + h2 / 2.);

	//Predictor
		k0 = (23. * k1 - 16. * k2 + 5. * k3) / 12.;
		h0 = (23. * h1 - 16. * h2 + 5. * h3) / 12.;

	//Corrector
		velocity += (9. * k0 + 19. * k1 - 5. * k2 + k3) / 24.;
		position += (9. * h0 + 19. * h1 - 5. * h2 + h3) / 24.;
}

void Solver::Solve() {

	//save initial position
	Vector3D initial_pos = Particle.position;

	for (time = 0; time < T; time += step) {

		grav_forces.Zero();
		distance.Zero();
		Particle.PotentialEnergy.Zero();


		if (!Planets.empty()) { 
			
			Calculate_Grav();
		}	
			//use forces only if particle has a mass 
		if (Particle.mass > 0) {
			
			Calculate_Net();

			//combine xyz into one
			switch (method) {
			case adams:
				Adams_Bashford(time, Particle.velocity.x, Particle.position.x, step, Particle.force.x, Particle.mass); //x
				Adams_Bashford(time, Particle.velocity.y, Particle.position.y, step, Particle.force.y, Particle.mass); //y
				Adams_Bashford(time, Particle.velocity.z, Particle.position.z, step, Particle.force.z, Particle.mass); //z
				break;
			case euler:
				Euler(time, Particle.velocity.x, Particle.position.x, step, Particle.force.x, Particle.mass); 
				Euler(time, Particle.velocity.y, Particle.position.y, step, Particle.force.y, Particle.mass); 
				Euler(time, Particle.velocity.z, Particle.position.z, step, Particle.force.z, Particle.mass); 
				break;
			case midpoint:
				Midpoint(time, Particle.velocity.x, Particle.position.x, step, Particle.force.x, Particle.mass); 
				Midpoint(time, Particle.velocity.y, Particle.position.y, step, Particle.force.y, Particle.mass); 
				Midpoint(time, Particle.velocity.z, Particle.position.z, step, Particle.force.z, Particle.mass); 
				break;
			case runge:
				Runge_Kutta(time, Particle.velocity.x, Particle.position.x, step, Particle.force.x, Particle.mass); 
				Runge_Kutta(time, Particle.velocity.y, Particle.position.y, step, Particle.force.y, Particle.mass); 
				Runge_Kutta(time, Particle.velocity.z, Particle.position.z, step, Particle.force.z, Particle.mass); 
				break;
			}
		}

		else {
			Particle.position += Particle.velocity; //if no mass only update position and displacement
		}

		//displacement
		Particle.displacement = { Particle.position.x - initial_pos.x, Particle.position.y - initial_pos.y, Particle.position.z - initial_pos.z };

		//kinetic energy 
		Particle.KineticEnergy = { Particle.velocity.x * Particle.mass / 2, Particle.velocity.y * Particle.mass / 2 , Particle.velocity.z * Particle.mass / 2 };
		
		//save values to vectors
		Push_Back();
	}

	//display information at the end of motion:
	std::cout << "Result: \n" << "\nTime of motion: " << T << " seconds" << std::endl;
	Particle.Print_info();
	Print_Pauses();
	
}



void Solver::Push_Back() {

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
		<< " EPotx[J]" << " EPoty[J]" << " EPotz[J]" 
		<< " Method" << std::endl;

	for (auto step = 0; step < time_data.size(); step++) {

		file << time_data[step] << " " << mass_data[step] << " " << fuel_data[step] << " " << Particle.fuel_usage
			<< " " << position_data[step].x << " " << position_data[step].y << " " << position_data[step].z
			<< " " << velocity_data[step].x << " " << velocity_data[step].y << " " << velocity_data[step].z
			<< " " << engine_data[step].x << " " << engine_data[step].y << " " << engine_data[step].z
			<< " " << force_data[step].x << " " << force_data[step].y << " " << force_data[step].z 
			<< " " << kinetic_data[step].x << " " << kinetic_data[step].y << " " << kinetic_data[step].z
			<< " " << potential_data[step].x << " " << potential_data[step].y << " " << potential_data[step].z
			<< " " << method << std::endl;
	}
	file.close();
}


