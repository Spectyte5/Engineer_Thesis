#pragma once
#include "Vector3D.h"

class Planet {

public:

	double mass = 0;
	double radius = 0;
	Vector3D position = { 0,0,0 };
	std::string name = "";
	
	//default 
	Planet() {}

	//object constructor
	Planet(double m, double r, double x, double y, double z, std::string n) {
		mass = m;
		radius = r;
		name = n;
	}

	void Print_info(){
		std::cout << "Name: " << name << "\nMass: " << mass << "\nRadius: " << radius  << "\nPosition: " << position << std::endl;
	}

	void Change_info() {

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
	}

		//Constructor for user created single planet
		Planet(bool single) {
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
		}
};