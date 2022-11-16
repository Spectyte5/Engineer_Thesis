#include "pch.h"
#include "../Engineer_Thesis/Solver.cpp"
#include "../Engineer_Thesis/Vehicle.cpp"

class SolverTest : public testing::Test {
public:
	Solver solver;
	std::streambuf* old_buffer = std::cout.rdbuf(nullptr); //disable cout during tests

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
		if (solver.Ship.mass != m) return false;
		
		//position
		if (!solver.Ship.position.VectorsEqual({ rx, ry, rz })) return false;

		//add displacement check

		//velocities
		if (!solver.Ship.velocity.VectorsEqual({ vx, vy, vz })) return false;

		//engine
		if (!solver.Ship.engine.VectorsEqual({ ex, ey, ez })) return false;

		//forces
		if (!solver.Ship.force.VectorsEqual({ fx, fy, fz })) return false;

		return true;
	}

	void CheckValues(Vector3D position, Vector3D displacement, Vector3D velocity, Vector3D engine, Vector3D force, double mass, double tolerance) {

		//mass
		ASSERT_DOUBLE_EQ(solver.Ship.mass,mass);

		//position
		ASSERT_NEAR(solver.Ship.position.x, position.x, tolerance);
		ASSERT_NEAR(solver.Ship.position.y, position.y, tolerance);
		ASSERT_NEAR(solver.Ship.position.z, position.z, tolerance);
		
		//displacement
		ASSERT_NEAR(solver.Ship.displacement.x, displacement.x, tolerance);
		ASSERT_NEAR(solver.Ship.displacement.y, displacement.y, tolerance);
		ASSERT_NEAR(solver.Ship.displacement.z, displacement.z, tolerance);

		//velocities
		ASSERT_NEAR(solver.Ship.velocity.x, velocity.x, tolerance);
		ASSERT_NEAR(solver.Ship.velocity.y, velocity.y, tolerance);
		ASSERT_NEAR(solver.Ship.velocity.z, velocity.z, tolerance);

		//engine
		ASSERT_NEAR(solver.Ship.engine.x, engine.x, tolerance);
		ASSERT_NEAR(solver.Ship.engine.y, engine.y, tolerance);
		ASSERT_NEAR(solver.Ship.engine.z, engine.z, tolerance);

		//forces
		ASSERT_NEAR(solver.Ship.force.x, force.x, tolerance);
		ASSERT_NEAR(solver.Ship.force.y, force.y, tolerance);
		ASSERT_NEAR(solver.Ship.force.z, force.z, tolerance);
	}

	void Set_Stats(double rx, double ry, double rz, double vx, double vy, double vz, double m, double f, double u) {

		solver.Ship = Vehicle("Test", rx, ry, rz, vx, vy, vz, m, f, u);
		//take fuel mass into account
		solver.Ship.mass += solver.Ship.fuel;
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

//Collistion Test
TEST_F(SolverTest, PlanetSurfacePosition) {
	SetUp(1, 10, 0);
	Set_Stats(5000, 0, 0, 1000, 0, 0, 1000, 0, 0);
	Engine_Set({ 0,0,0 }, { 0,0,0 }, { 0,0,0 });
	Planet_Set(100000, 5000, 0, 0, 0, "Col");
	ASSERT_FALSE(solver.Check_Collision(solver.Planets.front()));
}

TEST_F(SolverTest, NoCollisionPosition) {
	SetUp(1, 10, 0);
	Set_Stats(10000, 0, 0, 1000, 0, 0, 1000, 0, 0);
	Engine_Set({ 0,0,0 }, { 0,0,0 }, { 0,0,0 });
	Planet_Set(100000, 5000, 0, 0, 0, "Col");
	ASSERT_FALSE(solver.Check_Collision(solver.Planets.front()));
}

TEST_F(SolverTest, CollistionPosition) {
	SetUp(1, 10, 0);
	Set_Stats(2000, 0, 0, 1000, 0, 0, 1000, 0, 0);
	Engine_Set({ 0,0,0 }, { 0,0,0 }, { 0,0,0 });
	Planet_Set(100000, 5000, 0, 0, 0, "Col");
	ASSERT_TRUE(solver.Check_Collision(solver.Planets.front()));
}

//Adam's B tests
TEST_F(SolverTest, Adams_BashfordMassOnly) {
	SetUp(0);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, Adams_BashfordInitialVelocity) {
	SetUp(0);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, Adams_BashfordWithEngineNoPlanets) {
	SetUp(0.1, 50, 0);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50}, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5, 381.5, -120.7 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 1.5);
}

//Euler tests
TEST_F(SolverTest, EulerMassOnly) {
	SetUp(1);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerInitialVelocity) {
	SetUp(1);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerWithEngineNoPlanets) {
	SetUp(0.1, 50, 1);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50 }, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5, 381.5, -120.7 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 1.5);
}

//Midpoint tests
TEST_F(SolverTest, MidpointMassOnly) {
	SetUp(2);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, MidpointInitialVelocity) {
	SetUp(2);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, MidpointWithEngineNoPlanets) {
	SetUp(0.1, 50, 2);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50 }, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5, 381.5, -120.7 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 1.5);
}



//Runge-Kutta tests
TEST_F(SolverTest, RungeKuttaMassOnly) {
	SetUp(3);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, RungeKuttaInitialVelocity) {
	SetUp(3);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, RungeKuttaWithEngineNoPlanets) {
	SetUp(0.1, 50, 3);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50 }, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5, 381.5, -120.7 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 1.5);
}

TEST_F(SolverTest, RungeKuttaWithEngineWithPlanet) {
	SetUp(10, 550, 3);
	Set_Stats(6371000, 0, 0, 11200, 0, 0, 10000, 0, 0);
	Engine_Set({ 0,0,0 }, { 0,0,0 }, { 0,0,0 });
	Planet_Set(5972200000000000000000000.0, 6371000, 0, 0, 0, "Earth");
	solver.Solve();
	//CheckValues({ 163.5,0,0 }, { 63.5, 0, 0 }, { 2.5648, 0, 0 }, { 500, 0, 0 }, { 500, 0, 0 }, 9500, 1.5);
}
