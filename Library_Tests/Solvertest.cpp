#include "pch.h"
#include "../Engineer_Thesis/Solver.cpp"
#include "../Engineer_Thesis/Vehicle.cpp"

class SolverTest : public testing::Test {
public:
	Solver solver;
	std::streambuf* old_buffer = std::cout.rdbuf(nullptr); //disable console during tests

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

	void CheckValues(Vector3D position, Vector3D displacement, Vector3D velocity, Vector3D engine, Vector3D force, double mass, double tolerance) {

		//mass
		ASSERT_NEAR(solver.Ship.mass, mass, tolerance);

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
		Planet planet;
		planet.mass = m;
		planet.radius = r;
		planet.position = { x,y,z };
		planet.name = n;

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
	CheckValues({ 0,0,0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, Adams_BashfordInitialVelocity) {
	SetUp(0);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	CheckValues({ 100, 100, 100 }, { 100, 100, 100 }, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, Adams_BashfordWithEngineNoPlanets) {
	SetUp(0.1, 50, 0);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50}, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5685, 381.4111, -120.7802 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 1.5);
}

TEST_F(SolverTest, Adams_BashfordWithEngineWithPlanet) {
	SetUp(1, 465, 0);
	Set_Stats(0, 1738100.0, 0, 0, 0, 0, 2150, 2350, 5.05376);
	Engine_Set({ 0,0,0 }, { 0,465,0 }, { 0,15418.6,0 });
	Planet_Set(7.346e22, 1738100.0, 0, 0, 0, "Moon");
	solver.Solve();
	CheckValues({ 0,2030178,0 }, { 0, 292078, 0 }, { 0, 1566, 0 }, { 0, 15418.6, 0 }, { 0, 12852, 0 }, 2150, 0.7);
}

//Euler tests
TEST_F(SolverTest, EulerMassOnly) {
	SetUp(1);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	CheckValues({ 0,0,0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, EulerInitialVelocity) {
	SetUp(1);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	CheckValues({ 100, 100, 100 }, { 100, 100, 100 }, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, EulerWithEngineNoPlanets) {
	SetUp(0.1, 50, 1);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50 }, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5685, 381.4111, -120.7802 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 1.0);
}

TEST_F(SolverTest, EulerWithEngineWithPlanet) {
	SetUp(1, 465, 1);
	Set_Stats(0, 1738100.0, 0, 0, 0, 0, 2150, 2350, 5.05376);
	Engine_Set({ 0,0,0 }, { 0,465,0 }, { 0,15418.6,0 });
	Planet_Set(7.346e22, 1738100.0, 0, 0, 0, "Moon");
	solver.Solve();
	CheckValues({ 0,2031136,0 }, { 0, 293036, 0 }, { 0, 1566, 0 }, { 0, 15418.6, 0 }, { 0, 12854, 0 }, 2150, 1.0);
}

//Midpoint tests
TEST_F(SolverTest, MidpointMassOnly) {
	SetUp(2);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	CheckValues({ 0,0,0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, MidpointInitialVelocity) {
	SetUp(2);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	CheckValues({ 100, 100, 100 }, { 100, 100, 100 }, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, MidpointWithEngineNoPlanets) {
	SetUp(0.1, 50, 2);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50 }, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5685, 381.4111, -120.7802 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 1.6);
}

TEST_F(SolverTest, MidpointWithEngineWithPlanet) {
	SetUp(1, 465, 2);
	Set_Stats(0, 1738100.0, 0, 0, 0, 0, 2150, 2350, 5.05376);
	Engine_Set({ 0,0,0 }, { 0,465,0 }, { 0,15418.6,0 });
	Planet_Set(7.346e22, 1738100.0, 0, 0, 0, "Moon");
	solver.Solve();
	CheckValues({ 0,2031953,0 }, { 0, 293853, 0 }, { 0, 1566, 0 }, { 0, 15418.6, 0 }, { 0, 12856, 0 }, 2150, 1.0);
}



//Runge-Kutta tests
TEST_F(SolverTest, RungeKuttaMassOnly) {
	SetUp(3);
	Set_Stats(0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Solve();
	CheckValues({ 0,0,0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, RungeKuttaInitialVelocity) {
	SetUp(3);
	Set_Stats(0, 0, 0, 10, 10, 10, 1000, 0, 0);
	solver.Solve();
	CheckValues({ 100, 100, 100 }, { 100, 100, 100 }, { 10, 10, 10 }, { 0, 0, 0 }, { 0, 0, 0 }, 1000, 0.1);
}

TEST_F(SolverTest, RungeKuttaWithEngineNoPlanets) {
	SetUp(0.1, 50, 3);
	Set_Stats(100, -100, 100, 0, 0, 0, 9500, 500, 10);
	Engine_Set({ 0,0,0 }, { 50,50,50 }, { 500,3000,-950 });
	solver.Solve();
	CheckValues({ 163.5,281.5,-20.7 }, { 63.5685, 381.4111, -120.7802 }, { 2.5648, 15.3888, -4.873 }, { 500, 3000, -950 }, { 500, 3000, -950 }, 9500, 0.5);
}

TEST_F(SolverTest, RungeKuttaWithEngineWithPlanet) {
	SetUp(1, 465, 3);
	Set_Stats(0, 1738100.0, 0, 0, 0, 0, 2150, 2350, 5.05376);
	Engine_Set({ 0,0,0 }, { 0,465,0 }, { 0,15418.6,0 });
	Planet_Set(7.346e22, 1738100.0, 0, 0, 0, "Moon");
	solver.Solve();
	CheckValues({ 0,2030320,0 }, { 0, 292220, 0 }, { 0, 1566, 0 }, {0, 15418.6, 0 }, { 0, 12852, 0 }, 2150, 1.0);
}

