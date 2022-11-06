#include <iostream>
#include "Solver.h"

int main(int argc, char* argv[])
{
	Solver method;
	
		enum choice { create, load};
		std::string filename = "";
		char change = 'n';
		int option = -1;

		if (argc == 2){
			option = load; 
			filename = argv[1];
		}

		else {
			method.Print_Pauses();
			std::cout << "Welcome to C++ library for numerical calculations related to basic spacecraft motion issues." << std::endl;
			while (std::cout << "Please choose if you want to: \n 0.Create new simulation \n 1.Load simulation from JSON file " && !(std::cin >> option) || (option < 0 || option > 1)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid Selection\n";
			}

			switch (option) {
			case create:
				method.Setup();
				method.Save_json();
				method.Solve();
				method.Save_data();
				break;
			case load:
				method.Load_data(filename);
				method.Solve();
				method.Save_data();
				break;
			default:
				std::cout << "Invalid Selection\n";
				method.Print_Pauses();
				break;
			}
		}
}
