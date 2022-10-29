#include <iostream>
#include "Solver.h"

int main(int argc, char* argv[])
{
	Solver solver;
	
		enum choice { create, load, json };
		std::string filename = "";
		char change = 'n';
		int option = -1;

		if (argc == 2){
			option = json; 
			filename = (std::string)argv[1];
		}

		else {

			std::cout << "Welcome to C++ library for numerical calculations related to basic spacecraft motion issues." << std::endl;
			while (std::cout << "Please choose if you want to: \n 0.Create new simulation \n 1.Load an existing simulation \n 2.Load from JSON file " && !(std::cin >> option) || (option < 0 || option > 2)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid Selection\n";
			}

			switch (option) {
			case create:
				solver.Particle.User_set();
				solver.Setup();
				solver.Euler();
				solver.Save_data();
				solver.Save_planets();
				break;
			case load:
				solver.Load_data();
				solver.Load_planets();
				std::cout << "\nDo you want to change parameters of the ship? [y/n]" << std::endl;
				std::cin >> change;
				if (change == 'y') {
					solver.Change_data();
				}
				change = 'n';
				std::cout << "\nDo you want to change configuration of planets? [y/n]" << std::endl;
				std::cin >> change;
				if (change == 'y') {
					solver.Change_planets();
				}
				solver.Euler();
				solver.Save_data();
				solver.Save_planets();
				break;
			case json:
				solver.Load_json(filename);
				solver.Euler();
				solver.Save_data();
				solver.Save_planets();
				break;
			default:
				std::cout << "Invalid Selection\n";
				break;
			}
		}
}
