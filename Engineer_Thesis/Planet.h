#pragma once
#include "Vector3D.h"

class Planet {

public:

	double mass = 0;
	double radius = 0;
	double inertia = 0;
	Vector3D position = { 0,0,0 };
	std::string name = "";
	
	//constructor calculating inertia for perfect sphere
	Planet(double m, double r, double x, double y, double z, std::string n) {
		mass = m;
		radius = r;
		inertia = 0.4 * mass * radius * radius;
		name = n;
	}
	//constructor with custom inertia
	Planet(double m, double r, double x, double y, double z, std::string n ,double i) {
		mass = m;
		radius = r;
		inertia = i;
		position = { x, y, z };
	}

	void Print_info(){
		std::cout << "Name: " << name << "\nMass: " << mass << "\nRadius: " << radius << "\nInertia: " << inertia << "\nPosition: " << position << std::endl;
	}

	void Change_info() {
		bool CalculateInertia = false;
		char choice = 'n';
		std::cout << "Data changing of the particle ";
		std::cout << "Do you wish to change name? [y/n]";
		std::cin >> choice;
		if (choice == 'y') {
			std::cout << "Enter name of the planet: ";
			std::cin >> name;
		}
		std::cout << "Do you wish to change mass? [y/n]";
		std::cin >> choice;
		if (choice == 'y') {
			while (std::cout << "Enter mass of the planet: " && !(std::cin >> mass) || (mass < 0)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}
		}

		std::cout << "Do you wish to change radius? [y/n]";
		std::cin >> choice;
		if (choice == 'y') {
			while (std::cout << "Enter radius of the planet: " && !(std::cin >> radius) || (radius < 0)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}
		}

		std::cout << "Do you wish to change position? [y/n]";
		std::cin >> choice;
		if (choice == 'y') {

			std::cout << "Enter position x,y,z of the planet: ";
			std::cin >> position.x >> position.y >> position.z;
		}

		std::cout << "Do you wish to change position? [y/n]";
		std::cin >> choice;
		if (choice == 'y') {

			std::cout << "Enter position x,y,z of the planet: ";
			std::cin >> position.x >> position.y >> position.z;
		}

		std::cout << "Do you wish to change inertia? [y/n]";
		std::cin >> choice;
		if (choice == 'y') {
			while (std::cout << "Enter inertia of the planet: " && !(std::cin >> inertia) || (inertia < 0)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}
		}
		else {
			inertia = 0.4 * mass * radius * radius;
		}
	}

		//Constructor for user created single planet
		Planet() {
			bool CalculateInertia = false;
			std::cout << "Enter name of the planet: ";
			std::cin >> name;

			while (std::cout << "Enter mass of the planet: " && !(std::cin >> mass) || (mass < 0)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}

			while (std::cout << "Enter radius of the planet: " && !(std::cin >> radius) || (radius < 0)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}

			std::cout << "Enter position x,y,z of the planet: ";
			std::cin >> position.x >> position.y >> position.z;


			while (std::cout << "Do you want to input inertia?\n 0 = NO \n 1 = YES " && !(std::cin >> CalculateInertia)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid input; please re-enter.\n";
			}

			if (CalculateInertia) {

				while (std::cout << "Enter inertia of the planet: " && !(std::cin >> inertia) || (inertia < 0)) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid input; please re-enter.\n";
				}
			}
			else {
				inertia = 0.4 * mass * radius * radius;
			}
		}
};