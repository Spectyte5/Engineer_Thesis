#include "pch.h"
#include "../Engineer_Thesis/Vector3D.cpp"

class Vector3DTest : public testing::Test {
public:
	Vector3D V1, V2;
	void SetUp(double v1x, double v1y, double v1z) {
		V1 = { v1x,v1y,v1z };
		V2 = { 0, 0, 0 };
	}

	void SetUp(double v1x, double v1y, double v1z, double v2x, double v2y, double v2z) {
		V1 = { v1x,v1y,v1z };
		V2 = { v2x,v2y,v2z };
	}
	void TearDown() {}
}; 


//Add
TEST_F(Vector3DTest, HandlesFunctionAdd) {

	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1.Add(V2);
	ASSERT_TRUE(result.VectorsEqual({ 0,0,0 }));

	//two positive vectors
	SetUp(0.2, 0.2, 0.2, 0.3, 0.3, 0.3);
	result = V1.Add(V2);
	ASSERT_TRUE(result.VectorsEqual({ 0.5, 0.5, 0.5 }));

	//two negative vectors
	SetUp(-10, -20, -30, -30, -20, -10);
	result = V1.Add(V2);
	ASSERT_TRUE(result.VectorsEqual({ -40,-40,-40 }));

	//different sign vectors
	SetUp(10, -20, 30, -10, 30, -50);
	result = V1.Add(V2);
	ASSERT_TRUE(result.VectorsEqual({ 0,10,-20 }));

}

//Substract
TEST_F(Vector3DTest, HandlesFunctionSubstract) {

	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1.Subtract(V2);
	ASSERT_TRUE(result.VectorsEqual({ 0,0,0 }));

	//two positive vectors
	SetUp(0.5, 0.5, 0.5, 0.2, 0.2, 0.2);
	result = V1.Subtract(V2);
	ASSERT_TRUE(result.VectorsEqual({0.3, 0.3, 0.3 }));

	//two negative vectors
	SetUp(-3, -2, -3, -1, -2, -1);
	result = V1.Subtract(V2);
	ASSERT_TRUE(result.VectorsEqual({ -2, 0, -2 }));

	//different sign vectors
	SetUp(-3, -3, -3, 1, 1, 1);
	result = V1.Subtract(V2);
	ASSERT_TRUE(result.VectorsEqual({ -4,-4,-4}));
}

//Multiply
TEST_F(Vector3DTest, HandlesFunctionMultiply) {

	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1.Multiply(V2);
	ASSERT_TRUE(result.VectorsEqual({ 0,0,0 }));

	//two positive vectors
	SetUp(0.2, 0.2, 0.2, 0.3, 0.3, 0.3);
	result = V1.Multiply(V2);
	ASSERT_TRUE(result.VectorsEqual({ 0.06,0.06,0.06 }));

	//two negative vectors
	SetUp(-10, -20, -30, -30, -20, -10);
	result = V1.Multiply(V2);
	ASSERT_TRUE(result.VectorsEqual({ 300,400,300 }));

	//different sign vectors
	SetUp(10, -20, 30, -10, 30, -50);
	result = V1.Multiply(V2);
	ASSERT_TRUE(result.VectorsEqual({ -100,-600,-1500 }));

}

//Divide
TEST_F(Vector3DTest, HandlesFunctionDivide) {

	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1.Divide(V2);
	ASSERT_FALSE(result.VectorsEqual({ 0,0,0 }));

	//two negative vectors
	SetUp(-20, -10, -5, -5, -5, -5);
	result = V1.Divide(V2);
	ASSERT_TRUE(result.VectorsEqual({ 4,2,1 }));

	//two positive vectors
	SetUp(0.8, 0.8, 0.8, 0.1, 0.1, 0.1);
	result = V1.Divide(V2);
	ASSERT_TRUE(result.VectorsEqual({ 8,8,8 }));

	//different sign vectors
	SetUp(-20, -10, -5, 5, 5, 5);
	result = V1.Divide(V2);
	ASSERT_TRUE(result.VectorsEqual({ -4,-2,-1 }));

}

TEST_F(Vector3DTest, HandlesOperatorPlus) {
	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1 + V2;
	ASSERT_TRUE(result.VectorsEqual({ 0,0,0 }));

	//two positive vectors
	SetUp(0.2, 0.2, 0.2, 0.3, 0.3, 0.3);
	result = V1 + V2;
	ASSERT_TRUE(result.VectorsEqual({ 0.5,0.5,0.5 }));

	//two negative vectors
	SetUp(-10, -20, -30, -30, -20, -10);
	result = V1 + V2;
	ASSERT_TRUE(result.VectorsEqual({ -40,-40,-40 }));

	//different sign vectors
	SetUp(10, -20, 30, -10, 30, -50);
	result = V1 + V2;
	ASSERT_TRUE(result.VectorsEqual({ 0,10,-20 }));
}

TEST_F(Vector3DTest, HandlesOperatorMinus) {
	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1 - V2;
	ASSERT_TRUE(result.VectorsEqual({ 0,0,0 }));

	//two positive vectors
	SetUp(0.5, 0.5, 0.5, 0.2, 0.2, 0.2);
	result = V1 - V2;
	ASSERT_TRUE(result.VectorsEqual({ 0.3, 0.3, 0.3 }));

	//two negative vectors
	SetUp(-3, -2, -3, -1, -2, -1);
	result = V1 - V2;
	ASSERT_TRUE(result.VectorsEqual({ -2, 0, -2 }));

	//different sign vectors
	SetUp(-3, -3, -3, 1, 1, 1);
	result = V1 - V2;
	ASSERT_TRUE(result.VectorsEqual({ -4,-4,-4 }));
}

TEST_F(Vector3DTest, HandlesOperatorStar) {
	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1 * V2;
	ASSERT_TRUE(result.VectorsEqual({ 0,0,0 }));

	//two positive vectors
	SetUp(0.2, 0.2, 0.2, 0.3, 0.3, 0.3);
	result = V1 * V2;
	ASSERT_TRUE(result.VectorsEqual({ 0.06,0.06,0.06 }));

	//two negative vectors
	SetUp(-10, -20, -30, -30, -20, -10);
	result = V1 * V2;
	ASSERT_TRUE(result.VectorsEqual({ 300,400,300 }));

	//different sign vectors
	SetUp(10, -20, 30, -10, 30, -50);
	result = V1 * V2;
	ASSERT_TRUE(result.VectorsEqual({ -100,-600,-1500 }));
}

TEST_F(Vector3DTest, HandlesOperatorSlash) {
	//two zero vectors
	SetUp(0, 0, 0, 0, 0, 0);
	Vector3D result = V1 / V2;
	ASSERT_FALSE(result.VectorsEqual({ 0,0,0 }));

	//two negative vectors
	SetUp(-20, -10, -5, -5, -5, -5);
	result = V1 / V2;
	ASSERT_TRUE(result.VectorsEqual({ 4,2,1 }));

	//two positive vectors
	SetUp(0.8, 0.8, 0.8, 0.1, 0.1, 0.1);
	result = V1 / V2;
	ASSERT_TRUE(result.VectorsEqual({ 8,8,8 }));

	//different sign vectors
	SetUp(-20, -10, -5, 5, 5, 5);
	result = V1 / V2;
	ASSERT_TRUE(result.VectorsEqual({ -4,-2,-1 }));
}


TEST_F(Vector3DTest, HandlesOperatorPlusEq) {

	SetUp(1, 1, 1);
	V1 += V1;
	ASSERT_TRUE(V1.VectorsEqual({ 2,2,2 }));

	SetUp(2, 2, 2, -1, -2, -0.3);
	V1 += V2;
	ASSERT_TRUE(V1.VectorsEqual({ 1,0,1.7 }));
}

TEST_F(Vector3DTest, HandlesOperatorMinusEq) {

	SetUp(1, 1, 1);
	V1 -= V1;
	ASSERT_TRUE(V1.VectorsEqual({ 0, 0, 0 }));

	SetUp(2, 2, 2, -1, -2, -0.3);
	V1 -= V2;
	ASSERT_TRUE(V1.VectorsEqual({ 3,4,2.3 }));
}

TEST_F(Vector3DTest, HandlesOperatorStarEq) {

	SetUp(1, 1, 1);
	V1 *= V1;
	ASSERT_TRUE(V1.VectorsEqual({ 1,1,1 }));

	SetUp(2, 2, 2, -1, -2, -0.3);
	V1 *= V2;
	ASSERT_TRUE(V1.VectorsEqual({ -2,-4,-0.6 }));
}

TEST_F(Vector3DTest, HandlesOperatorSlashEq) {

	SetUp(1, 1, 1);
	V1 /= V1;
	ASSERT_TRUE(V1.VectorsEqual({ 1,1,1 }));

	SetUp(2, 2, 2, -1, -2, -0.4);
	V1 /= V2;
	ASSERT_TRUE(V1.VectorsEqual({ -2,-1,-5}));
}

//Multiplying by a scalar
TEST_F(Vector3DTest, HandlesScalarMultiplying) {

	Vector3D result;

	//positive int
	SetUp(1, 1, 1);
	result = V1 * 5;
	ASSERT_TRUE(result.VectorsEqual({ 5,5,5 }));

	//negative int
	SetUp(1, 1, 1);
	result = V1 * (-2);
	ASSERT_TRUE(result.VectorsEqual({ -2,-2,-2 }));

	//zero
	SetUp(1, 1, 1);
	result = V1 * 0;
	ASSERT_TRUE(result.VectorsEqual({ 0,0,0 }));
}

//Zero function
TEST_F(Vector3DTest, HandlesZeroingVectors) {
	
	SetUp(0, 0, 0);
	V1.Zero();
	ASSERT_TRUE(V1.VectorsEqual({ 0,0,0 }));

	SetUp(-555, 220, 110);
	V1.Zero();
	ASSERT_TRUE(V1.VectorsEqual({ 0,0,0 }));

	SetUp(122, -3, 54);
	V1.Zero();
	ASSERT_TRUE(V1.VectorsEqual({ 0,0,0 }));
}

int main(int argc, char** argv) {

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}