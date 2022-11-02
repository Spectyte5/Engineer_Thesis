#include "pch.h"
#include "../Engineer_Thesis/Solver.cpp"
#include "../Engineer_Thesis/Vehicle.cpp"

class SolverTest : public testing::Test {
public:
	Solver solver;

	void SetUp() {
		solver.T = 10;
		solver.step = 1;
	}

	void SetUp(double step, double time) {
		 solver.T = time;
		 solver.step = step;
	} 

	bool CheckShip(double rx, double ry, double rz, double vx, double vy, double vz, double ex, double ey, double ez, double fx, double fy, double fz, double m) {

		//mass
		if (solver.Particle.mass != m) return false;
		
		//position
		if (!solver.Particle.position.VectorsEqual({ rx, ry, rz })) return false;

		//velocities
		if (!solver.Particle.velocity.VectorsEqual({ vx, vy, vz })) return false;

		//engine
		if (!solver.Particle.engine.VectorsEqual({ ex, ey, ez })) return false;

		//forces
		if (!solver.Particle.force.VectorsEqual({ fx, fy, fz })) return false;

		return true;
	}

	void Set_Stats(double rx, double ry, double rz, double vx, double vy, double vz, double m, double f, double u) {

		solver.Particle = Point_Particle("Test", rx, ry, rz, vx, vy, vz, m, f, u);
		//take fuel mass into account
		solver.Particle.mass += solver.Particle.fuel;
	}

	void Engine_Set(Vector3D timestart, Vector3D timeend, Vector3D force) {

		Control interval;
		interval.timestart = timestart;
		interval.timeend = timeend;
		interval.engforce = force;
		solver.TimeVect.push_back(interval);
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
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionNoEngineNoPlanets) {
	SetUp();
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionWithEngineNoFuelNoPlanets) {
	SetUp();
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	Engine_Set({10,10,10}, {10,10,10}, {2000, 2000, 2000});
	solver.Euler();
	ASSERT_TRUE(CheckShip(2, 2, 2, 2, 2, 2, 2000, 2000, 2000, 2000, 2000, 2000, 1000));
}
