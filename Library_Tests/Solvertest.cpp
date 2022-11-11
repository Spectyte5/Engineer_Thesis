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

	void SetUp(int ode) {
		solver.T = 10;
		solver.step = 1;
		solver.method = ode;
	}

	void SetUp(double step, double time) {
		solver.T = time;
		solver.step = step;
	} 

	void SetUp(double step, double time, int ode) {
		solver.T = time;
		solver.step = step;
		solver.method = ode;
	}

	bool CheckShip(double rx, double ry, double rz, double vx, double vy, double vz, double ex, double ey, double ez, double fx, double fy, double fz, double m) {

		//mass
		if (solver.Particle.mass != m) return false;
		
		//position
		if (!solver.Particle.position.VectorsEqual({ rx, ry, rz })) return false;

		//add displacement check

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


//Adam's B tests

TEST_F(SolverTest, Adams_BashfordFunctionMassOnly) {
	SetUp(0);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, Adams_BashfordFunctionNoEngineNoPlanets) {
	SetUp(0);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, Adams_BashfordFunctionWithEngineNoPlanets) {
	SetUp(0.1, 50, 0);
	Set_Stats(0, 0, 0, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,0,0}, { 500,0,0 });
	solver.Solve();
	ASSERT_NEAR(solver.Particle.velocity.x, 2.5648, 0.001);
}

//Euler tests
TEST_F(SolverTest, EulerFunctionMassOnly) {
	SetUp(1);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionNoEngineNoPlanets) {
	SetUp(1);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionWithEngineNoPlanets) {
	SetUp(0.1, 50, 1);
	Set_Stats(0, 0, 0, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,0,0 }, { 500,0,0 });
	solver.Solve();
	ASSERT_NEAR(solver.Particle.velocity.x, 2.5648, 0.001);
}

//Midpoint tests

TEST_F(SolverTest, MidpointFunctionMassOnly) {
	SetUp(2);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, MidpointFunctionNoEngineNoPlanets) {
	SetUp(2);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, MidpointFunctionWithEngineNoPlanets) {
	SetUp(0.1, 50, 2);
	Set_Stats(0, 0, 0, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,0,0 }, { 500,0,0 });
	solver.Solve();
	ASSERT_NEAR(solver.Particle.velocity.x, 2.5648, 0.001);
}

//Runge-Kutta tests

TEST_F(SolverTest, RungeKuttaFunctionMassOnly) {
	SetUp(3);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, RungeKuttaFunctionNoEngineNoPlanets) {
	SetUp(3);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, RungeKuttaFunctionWithEngineNoPlanets) {
	SetUp(0.1, 50, 3);
	Set_Stats(0, 0, 0, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,0,0 }, { 500,0,0 });
	solver.Solve();
	ASSERT_NEAR(solver.Particle.velocity.x, 2.5648, 0.001);
}
