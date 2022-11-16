#include <iostream>
#include "Solver.h"

int main(int argc, char* argv[])
{
	Solver solver;
	
		enum choice { create, load};
		std::string filename = "";
		char change = 'n';
		int option = -1;

		if (argc == 2){
			option = load; 
			filename = argv[1];
		}

		else {
			solver.Print_Pauses();
			std::cout << "Welcome to C++ library for numerical calculations related to basic spacecraft motion issues." << std::endl;
			while (std::cout << "Please choose if you want to: \n 0.Create new simulation \n 1.Load simulation from JSON file " && !(std::cin >> option) || (option < 0 || option > 1)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid Selection\n";
			}

			switch (option) {
			case create:
				solver.Setup();
				solver.Save_json();
				solver.Solve();
				solver.Save_data();
				break;
			case load:
				solver.Load_data(filename);
				solver.Solve();
				solver.Save_data();
				break;
			default:
				std::cout << "Invalid Selection\n";
				solver.Print_Pauses();
				break;
			}
		}
}
