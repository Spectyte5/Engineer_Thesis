#include "pch.h"
#include "../Engineer_Thesis/Solver.cpp"
#include "../Engineer_Thesis/Vehicle.cpp"

class SolverTest : public testing::Test {
public:
	Solver solver;
	 void SetUp() {
		double T = 10, t = 1, tx = 0, ty = 0, tz = 0;
	} 

	void SetUp(int engine) {
		double T = 20, t = 0.5, tx = 10, ty = 10, tz = 10;
	}

	void SetUp(bool longertime) {
		double T = 30, t = 0.2, tx = 0, ty = 0, tz = 0;
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

	void TearDown() {}
};



//Euler tests
TEST_F(SolverTest, EulerFunctionMassOnly) {
	SetUp();
	solver.Particle = Point_Particle("Test", 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(0, 0, 0, 0, 0, 0, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionNoEngineNoPlanets) {
	SetUp();
	solver.Particle = Point_Particle("Test", 0, 0, 0, 10, 10, 10, 0, 0, 0, 1000, 0, 0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 1000));
}

TEST_F(SolverTest, EulerFunctionWithEngineNoPlanets) {
	SetUp(2);
	solver.Particle = Point_Particle("Test", 0, 0, 0, 10, 10, 10, 10, 10, 10, 1000, 0, 0);
	solver.Euler();
	ASSERT_TRUE(CheckShip(100, 100, 100, 10, 10, 10, 0, 0, 0, 1000));
}


