#include "pch.h"
#include "../Engineer_Thesis/Solver.cpp"
#include "../Engineer_Thesis/Vehicle.cpp"

class SolverTest : public testing::Test {
public:
	Solver solver;

	void SetUp() {
		solver.T = 10;
		solver.t = 1;
		solver.tx = 0;
		solver.ty = 0;
		solver.tz = 0;
	}

	 void SetUp(double time_interval, double time) {
		 solver.T = time;
		 solver.t = time_interval;
	     solver.tx = 0; 
	     solver.ty = 0;
		 solver.tz = 0;
	} 

	 void SetUp(double time_interval, double time, double time_x, double time_y, double time_z) {
		 solver.T = time;
		 solver.t = time_interval;
		 solver.tx = time_x;
		 solver.ty = time_y;
		 solver.tz = time_z;
	 }

	bool CheckShip(double rx, double ry, double rz, double vx, double vy, double vz, double fx, double fy, double fz, double m) {

		//mass
		if (solver.Particle.mass != m) return false;
		
		//position
		if (!solver.Particle.position.VectorsEqual({ rx, ry, rz })) return false;

		//velocities
		if (!solver.Particle.velocity.VectorsEqual({ vx, vy, vz })) return false;

		//forces
		if (!solver.Particle.force.VectorsEqual({ fx, fy, fz })) return false;
		return true;
	}

	void Set_Stats(double rx, double ry, double rz, double vx, double vy, double vz, double fx, double fy, double fz, double m, double f, double u) {

		solver.Particle = Point_Particle("Test", rx, ry, rz, vx, vy, vz, fx, fy, fz, m, f, u);
		//take fuel mass into account
		solver.Particle.mass += solver.Particle.fuel;
	}

	void Planet_Set(double m, double r, double x, double y, double z, std::string n) {

		//create a planet
		Planet planet(m, r, x, y, z, n);

		//push to the vector
		solver.Planets.push_back(planet);
	}

	void TearDown() {}
};


//Euler tests
TEST_F(SolverTest, EulerFunctionMassOnly) {
	SetUp();
	Set_Stats(0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionNoEngineNoPlanets) {
	SetUp(1, 10);
	Set_Stats(0, 0, 0, 10, 10, 10, 0, 0, 0, 1000, 0, 0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionWithEngineNoPlanets) {
	SetUp(1, 10, 10, 10, 10);
	Set_Stats(0, 0, 0, 0, 0, 0, 2000, 2000, 2000, 500, 500, 100);
	solver.Euler();
	ASSERT_TRUE(CheckShip(2, 2, 2, 2, 2, 2, 2000, 2000, 2000, 900));
}
//fix:
/*
TEST_F(SolverTest, EulerFunctionNoEngineOnePlanet) {
	SetUp(1, 2);
	Set_Stats(0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0);
	Planet_Set(59722 * 10^20, 6378.137, 100, 100, 100, "Test_Planet");
	solver.Push_Back(0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 1000));
}


TEST_F(SolverTest, EulerFunctionWithEngineOnePlanet) {
	SetUp(1, 1, 0, 0, 0);
	Set_Stats(0, 0, 0, 0, 0, 0, -2000, -2000, -2000, 500, 500, 100);
	Planet_Set(1000000, 5000, 1000, 1000, 1000, "Test_Planet");
	solver.Push_Back(0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(2, 2, 2, 2, 2, 2, 2000, 2000, 2000, 900));
	//data 0 15000 5000 10 0 0 0 10 10 -10 5 5 5
}
*/

//TODO: Test One and More Planets and Check loading from files and if data is the same as in creation
