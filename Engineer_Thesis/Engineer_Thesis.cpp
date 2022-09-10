#include <iostream>
#include "Solver.h"

int main()
{

	Point_Particle particle("", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	Solver solver;

	particle.User_set();
	solver.Euler(particle);
	solver.Save_data(particle);
}