#include "Solver.h"
#include <filesystem>
#include <json.hpp>
#include <valijson_nlohmann_bundled.hpp>

void Solver::Populate() {

	double i=0;
	short int Planet_Num=0;
	Planet planet; 

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

		while (std::cout << "Enter mass in kg of the planet " << i << " : " && !(std::cin >> planet.mass) || (planet.mass < 0)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input; please re-enter.\n";
		}
		
		while (std::cout << "Enter radius in m of the planet " << i << " : " && !(std::cin >> planet.radius) || (planet.radius < 0)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input; please re-enter.\n";
		}

		std::cout << "Does this planet orbit around a point XYZ? \n 0.No \n 1.Yes ";
		std::cin >> planet.isOrb;

		if (planet.isOrb) {
			while (std::cout << "Enter radius in m of the orbit : " && !(std::cin >> planet.orb_radius) || (planet.orb_radius < 0)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}
			 
			while (std::cout << "Enter initial angle on the orbit : " && !(std::cin >> planet.start_ang) || (planet.start_ang < 0) || (planet.start_ang > 6.29)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}

			while (std::cout << "Enter angular velocity of the orbiting in rad/s: " && !(std::cin >> planet.ang_velocity)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}

			std::cout << "Enter position x,z in m of the orbit center : ";
			std::cin >> planet.orb_pos.x >> planet.orb_pos.z;
		}
		else {
			std::cout << "Enter position x,y,z in m of the planet " << i << " : ";
			std::cin >> planet.position.x >> planet.position.y >> planet.position.z;
		}

		Planets.push_back(planet);
		std::cout << "\nPlanet Added: \n";
		planet.Print_info();
	}
}

void Solver::Setup() {
	Print_Pauses();
	Ship.User_set(); //setup particle
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

bool Solver::Validate_Json(std::string& filename) {

	using namespace valijson;
	using namespace valijson::adapters;
	using json = nlohmann::json;

	// Load JSON schema document with Valijson helper function
	json mySchemaDoc;
	if (!valijson::utils::loadDocument("./JSON_files/Schema/schema.json", mySchemaDoc)) {
		throw std::runtime_error("Failed to load schema document");
		return false;
	}

	// Parse JSON schema content using valijson
	Schema mySchema;
	SchemaParser parser;
	NlohmannJsonAdapter mySchemaAdapter(mySchemaDoc);
	parser.populateSchema(mySchemaAdapter, mySchema);
	
	//Load document to validate :
	json myTargetDoc;
	if (!valijson::utils::loadDocument(filename, myTargetDoc)) {
		throw std::runtime_error("Failed to load target document");
		return false;
	}

	// Perform validation
	Validator validator(Validator::kStrongTypes);
	ValidationResults results;
	NlohmannJsonAdapter targetDocumentAdapter(myTargetDoc);
	if (!validator.validate(mySchema, targetDocumentAdapter, &results)) {
		std::cerr << "Validation failed." << std::endl;
		ValidationResults::Error error;
		unsigned int errorNum = 1;
		while (results.popError(error)) {

			std::string context;
			std::vector<std::string>::iterator itr = error.context.begin();
			for (; itr != error.context.end(); itr++) {
				context += *itr;
			}

			std::cerr << "Error #" << errorNum << std::endl
				<< "  context: " << context << std::endl
				<< "  desc:    " << error.description << std::endl;
			++errorNum;
		}
		return false;
	}
	return true;
}


void Solver::Save_json() {

	using json = nlohmann::json;

	// create an empty structure
	json data;
	data["ship"]["name"] = Ship.name;
	data["ship"]["position"][0] = Ship.position.x;
	data["ship"]["position"][1] = Ship.position.y;
	data["ship"]["position"][2] = Ship.position.z;
	data["ship"]["velocity"][0] = Ship.velocity.x;
	data["ship"]["velocity"][1] = Ship.velocity.y;
	data["ship"]["velocity"][2] = Ship.velocity.z;
	data["ship"]["mass"] = Ship.mass;
	data["ship"]["fuel"] = Ship.fuel;
	data["ship"]["fuel_usage"] = Ship.fuel_usage;

	for (auto& p : Planets) {
		auto i = &p - &Planets[0];
		data["planets"][i]["name"] = p.name;
		data["planets"][i]["mass"] = p.mass;
		data["planets"][i]["radius"] = p.radius;
		data["planets"][i]["hasorbit"] = p.isOrb;
		if (p.isOrb) {
			data["planets"][i]["start_angle"] = p.start_ang;
			data["planets"][i]["orbit_radius"] = p.orb_radius;
			data["planets"][i]["ang_velocity"] = p.ang_velocity;
			data["planets"][i]["orbit_pos"][0] = p.orb_pos.x;
			data["planets"][i]["orbit_pos"][1] = p.orb_pos.z;
		}
		else {
			data["planets"][i]["position"][0] = p.position.x;
			data["planets"][i]["position"][1] = p.position.y;
			data["planets"][i]["position"][2] = p.position.z;
		}
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
	std::string path = "./JSON_files/" + Ship.name + ".json";
	std::ofstream file(path.c_str());
	file << std::setw(4) << data << std::endl;
	std::cout << "File saved Successfully as: " << path << std::endl;
	Print_Pauses();
}

std::ifstream Solver::Load_file(std::string sys_path, std::string filepath, std::string extenstion) {

	std::ifstream file;
	std::string filename, fullname;

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
		std::cin >> filename;
		fullname = filepath;
		fullname.append(filename + extenstion.c_str());
		file.open(fullname.c_str());
		if (!file) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			fullname.clear();
			std::cout << "\nFile not found\n" << std::endl;
		}
		else {
			break;
		}
	}

	if (extenstion == ".json") {
		if (!Validate_Json(fullname)) exit(0);
	}

	Print_Pauses();
	return file;
}

void Solver::Load_data(std::string& filename) {

	std::ifstream file;

	if (filename != "") {
		file.open(filename);
		if (!Validate_Json(filename)) exit(0);
	}

	else {
		file = Load_file(".\\JSON_files\\", "./JSON_files/", ".json");
	}

	using json = nlohmann::json;
	json data = json::parse(file);

	Ship.name = data["ship"]["name"];
	Ship.position = { data["ship"]["position"][0], data["ship"]["position"][1], data["ship"]["position"][2] };
	Ship.velocity = { data["ship"]["velocity"][0], data["ship"]["velocity"][1], data["ship"]["velocity"][2] };
	Ship.mass = data["ship"]["mass"];
	Ship.fuel = data["ship"]["fuel"];
	Ship.fuel_usage = data["ship"]["fuel_usage"];
	
	std::cout << "Ship:\n\n" << "Ship loaded: " << std::endl;
	Ship.Print_info();
	Print_Pauses();

	std::cout << "Planets: \n";
	for (auto& p : data["planets"]) {
		Planet planet;
		planet.name = p["name"];
		planet.mass = p["mass"];
		planet.radius = p["radius"];
		planet.isOrb = p["orbit"];
		if (planet.isOrb) {
			planet.start_ang = p["start_angle"];
			planet.orb_radius = p["orbit_radius"];
			planet.ang_velocity = p["ang_velocity"];
			planet.orb_pos = { p["orbit_pos"][0], 0, p["orbit_pos"][1]};
		}
		else planet.position = { p["position"][0], p["position"][1], p["position"][2] }; 
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


	double distance = pow(Ship.position.x - Planet.position.x,2) 
		+ pow(Ship.position.y - Planet.position.y, 2) 
		+ pow(Ship.position.z - Planet.position.z, 2);

	if (distance < (Planet.radius * Planet.radius)) {
		std::cout << "\n" << Ship.name << ", has crashed into Planet: " << Planet.name << " At time: " << time << " [s]"
			<< " !\n********************************************************" << std::endl;
		return true;
	}

	// distance btw centre and point is equal to radius
	else if (distance == (Planet.radius * Planet.radius)) {

		//if it is on surface it can be a start from the planet and a try to escape the orbit, so i return it as not a full collistion, 
		std::cout << "\n " << Ship.name << ", is located on " << Planet.name << "'s Surface !" << std::endl;
		return false;
	}

	// distance btw center and point is greater than radius
	else return false;
}

bool Solver::UseEngine() {

	bool used = false;
	//TODO: Check fuel

	for (auto& i : TimeVect) {

		used = false;
		
		//first check if there is fuel:
		if (Ship.fuel <= 0) {
			std::cout << "\nRun Out of fuel at time: " << time << ", engines won't be used from now on..." << std::endl;
			TimeVect.clear(); //if there is no fuel there is no reason to check engine usage
			break;
		}

		//check if simulation step is contained in one of the intervals and if first interval is not 0 sec long
		//x
		if (time >= i.timestart.x && time <= i.timeend.x && i.timeend.x != 0) {
			Ship.engine.x = i.engforce.x; //if yes turn on the engine
			used = true;
		}

		//y
		if (time >= i.timestart.y && time <= i.timeend.y && i.timeend.y != 0) {
			Ship.engine.y = i.engforce.y;
			used = true;
		}

		//z
		if (time >= i.timestart.z && time <= i.timeend.z && i.timeend.z != 0) {
			Ship.engine.z = i.engforce.z;
			used = true;
		}

		//because current time can only be in one interval we do not need to check all of them
		if (used) {
			return true;
		}
	}
	return false;
}

void Solver::Calculate_Grav() {

	double r;

	for (auto& p : Planets) {

		//check if ship collides with the planet and print that information
		Check_Collision(p);

		//distance between ship and planet
		r = sqrt(pow(Ship.position.x - p.position.x, 2)
			+ pow(Ship.position.y - p.position.y, 2)
			+ pow(Ship.position.z - p.position.z, 2));
		//xyz distance
		distance = { Ship.position.x - p.position.x, Ship.position.y - p.position.y, Ship.position.z - p.position.z };

		grav_forces.x -= (G * p.mass * Ship.mass * distance.x) / pow(r, 3);
		grav_forces.y -= (G * p.mass * Ship.mass * distance.y) / pow(r, 3);
		grav_forces.z -= (G * p.mass * Ship.mass * distance.z) / pow(r, 3);

		//Potential energy
		Ship.PotentialEnergy -= (G * p.mass * Ship.mass) / r;
	}
}

void Solver::Calculate_Net() {

	//will engine be used in this iteration 
	engine_used = false;
	//engine is used only if time intervals vector is not empty
	if (!TimeVect.empty()) {
		engine_used = UseEngine();
	}
	//remove fuel used 
	if (engine_used) {
		fuel_used = Ship.fuel_usage * step; //calculate fuel used
		Ship.fuel -= fuel_used;
		Ship.mass -= fuel_used;
	}
	//if engine not used
	else {
		Ship.engine.Zero();
	}
	//calculate net force
	Ship.force = { Ship.engine.x + grav_forces.x , Ship.engine.y + grav_forces.y, Ship.engine.z + grav_forces.z };
}

void Solver::Euler(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {
	
	velocity += dt * dvdt(time, velocity, force, mass);
	position += dt * dxdt(time, velocity, position);
}

void Solver::Midpoint(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {

	double mx, mv;
	mv = dt * dvdt(time, velocity, force, mass);
	mx = dt * dxdt(time, velocity, position);
	velocity += dt * dvdt(time + dt / 2., velocity + mv / 2., force, mass);
	position += dt * dxdt(time + dt / 2., velocity + mv / 2., position + mx / 2.);
}

//runge kutta method
void Solver::Runge_Kutta(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {

	double  k1, k2, k3, k4, h1, h2, h3, h4;

	k1 = dt * dvdt(time, velocity, force, mass);
	h1 = dt * dxdt(time, velocity, position);
	k2 = dt * dvdt(time + dt / 2., velocity + k1 / 2., force, mass);
	h2 = dt * dxdt(time + dt / 2., velocity + k1 / 2., position + h1 / 2.);
	k3 = dt * dvdt(time + dt / 2., velocity + k2 / 2., force, mass);
	h3 = dt * dxdt(time + dt / 2., velocity + k2 / 2., position + h2 / 2.);
	k4 = dt * dvdt(time + dt, velocity + k3, force, mass);
	h4 = dt * dxdt(time + dt, velocity + k3, position + h3);

	velocity += 1.0 / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);
	position += 1.0 / 6.0 * (h1 + 2 * h2 + 2 * h3 + h4);
}

//Adam - Bashforth method
void Solver::Adams_Bashford(double& time, double& velocity, double& position, double& dt, double& force, double& mass) {

	double  k0, k1, k2, k3, h0, h1, h2, h3;

	//Calculate initial steps RK4
		k1 = dt * dvdt(time, velocity, force, mass);
		h1 = dt * dxdt(time, velocity, position);
		k2 = dt * dvdt(time + dt / 2., velocity + k1 / 2., force, mass);
		h2 = dt * dxdt(time + dt / 2., velocity + k1 / 2., position + h1 / 2.);
		k3 = dt * dvdt(time + dt / 2., velocity + k2 / 2., force, mass);
		h3 = dt * dxdt(time + dt / 2., velocity + k2 / 2., position + h2 / 2.);

	//Predictor
	    k0 = (23. * k3 - 16. * k2 + 5. * k1) / 12.;
		h0 = (23. * h3 - 16. * h2 + 5. * h1) / 12.;

	//Corrector
		velocity += (5. * k0 + 8. * k3 - k2) / 12.;
		position += (5. * h0 + 8. * h3 - h2) / 12.;
}

void Solver::Solve() {

	//save initial position
	Vector3D initial_pos = Ship.position;

	for (time = 0; time < T; time += step) {

		grav_forces.Zero();
		distance.Zero();
		Ship.PotentialEnergy = 0;

		//move planets around orbit
		Move_Orbit();

		if (!Planets.empty()) { 
			Calculate_Grav();
		}	
			//use forces only if particle has a mass 
		if (Ship.mass > 0) {
			
			Calculate_Net();

			switch (method) {
			case adams:
				Adams_Bashford(time, Ship.velocity.x, Ship.position.x, step, Ship.force.x, Ship.mass); //x
				Adams_Bashford(time, Ship.velocity.y, Ship.position.y, step, Ship.force.y, Ship.mass); //y
				Adams_Bashford(time, Ship.velocity.z, Ship.position.z, step, Ship.force.z, Ship.mass); //z
				break;
			case euler:
				Euler(time, Ship.velocity.x, Ship.position.x, step, Ship.force.x, Ship.mass); 
				Euler(time, Ship.velocity.y, Ship.position.y, step, Ship.force.y, Ship.mass); 
				Euler(time, Ship.velocity.z, Ship.position.z, step, Ship.force.z, Ship.mass); 
				break;
			case midpoint:
				Midpoint(time, Ship.velocity.x, Ship.position.x, step, Ship.force.x, Ship.mass); 
				Midpoint(time, Ship.velocity.y, Ship.position.y, step, Ship.force.y, Ship.mass); 
				Midpoint(time, Ship.velocity.z, Ship.position.z, step, Ship.force.z, Ship.mass); 
				break;
			case runge:
				Runge_Kutta(time, Ship.velocity.x, Ship.position.x, step, Ship.force.x, Ship.mass); 
				Runge_Kutta(time, Ship.velocity.y, Ship.position.y, step, Ship.force.y, Ship.mass); 
				Runge_Kutta(time, Ship.velocity.z, Ship.position.z, step, Ship.force.z, Ship.mass); 
				break;
			}
		}

		else {
			Ship.position += Ship.velocity; //if no mass only update position and displacement
		}

		//displacement
		Ship.displacement = { Ship.position.x - initial_pos.x, Ship.position.y - initial_pos.y, Ship.position.z - initial_pos.z };

		//kinetic energy 
		Ship.KineticEnergy = Ship.mass / 2 * (pow(Ship.velocity.x, 2) + pow(Ship.velocity.y, 2) + pow(Ship.velocity.z, 2));
		Ship.CalculatedEnergy = true;

		//save values to vectors
		Push_Back();
	}

	//display information at the end of motion:
	std::cout << "Result: \n" << "\nTime of motion: " << T << " seconds" << std::endl;
	Ship.Print_info();
	Print_Pauses();
	
}

void Solver::Move_Orbit() {

	for (auto& p : Planets) {
		if (p.isOrb) {
			p.Move_Planet(time);
		}
	}
}

void Solver::Push_Back() {

	time_data.push_back(time);
	mass_data.push_back(Ship.mass);
	fuel_data.push_back(Ship.fuel);
	position_data.push_back(Ship.position);
	velocity_data.push_back(Ship.velocity);
	engine_data.push_back(Ship.engine);
	force_data.push_back(Ship.force);
	kinetic_data.push_back(Ship.KineticEnergy);
	potential_data.push_back(Ship.PotentialEnergy);
}

void Solver::Save_Planets() {

	std::string ship_name = Ship.name;
	std::string filename = "./Simulation_History/Planets/";
	ship_name.append("_Planets.txt");
	filename.append(ship_name);
	std::ofstream file;
	file.open(filename.c_str());
	file.precision(10);

	//write informational line
	file << std::fixed << " Name" << " isOrbiting " << " Mass[kg]" << " Radius[m]" << " Position.x[m]" << " Position.y[m]" << " Position.z[m]" << " Orbit Radius[m]" << " Orbit Velocity[m/s]" << " Orbit Initial angle[rad]" << std::endl;

	for (auto& p : Planets) {

		file << p.name << " " << p.isOrb << " " << p.mass << " "
	       << p.radius << " " << p.position.x << " " 
			<< p.position.y << " " << p.position.z << " "
			<< p.orb_radius << " " << p.ang_velocity << " " << p.start_ang
			<< std::endl;
	}

	file.close();

	std::string ship_orb = Ship.name;
	std::string fileorb = "./Simulation_History/Planets/Orbits/";
	std::string filenameorb;

	for (auto& p : Planets) {
		if (p.isOrb)
			filenameorb.append(fileorb + ship_orb + "_" + p.name + ".txt");
			file.open(filenameorb.c_str());
			file << std::fixed << " Position.x[m]" << " Position.y[m]" << std::endl;
			for (auto& v : p.orb_data) {
				file << v.x << " " << v.z << std::endl;
			}
			file.close();
	}


}

void Solver::Save_data() {

	std::string particle_name = Ship.name;
	std::string filename = "./Simulation_History/Ships/";
	particle_name.append(".txt");
	filename.append(particle_name);
	std::ofstream file;
	file.open(filename.c_str());
	file.precision(10);
	
	//write informational line
	file << std::fixed << " Time[s]" << " Mass[kg]" << " Fuel[kg]" << " FuelUsage[kg/s]" 
		<< " Position.x[m]" << " Position.y[m]" << " Position.z[m]" 
		<< " Velocity.x[m/s]" << " Velocity.y[m/s]" << " Velocity.z[m/s]"
		<< " Engine Force.x[N]" << " Engine Force.y[N]" << " Engine Force.z[N]"
		<< " Net Force.x[N]" << " Net Force.y[N]" << " Net Force.z[N]"
		<< " Kinetic Energy[J]" << " Potential Energy[J]"  << " Method" << std::endl;

	for (auto step = 0; step < time_data.size(); step++) {

		file << time_data[step] << " " << mass_data[step] << " " << fuel_data[step] << " " << Ship.fuel_usage
			<< " " << position_data[step].x << " " << position_data[step].y << " " << position_data[step].z
			<< " " << velocity_data[step].x << " " << velocity_data[step].y << " " << velocity_data[step].z
			<< " " << engine_data[step].x << " " << engine_data[step].y << " " << engine_data[step].z
			<< " " << force_data[step].x << " " << force_data[step].y << " " << force_data[step].z 
			<< " " << kinetic_data[step] << " " << potential_data[step] << " " << method << std::endl;
	}
	file.close();

	Save_Planets();
}


