#pragma once
#include <math.h>

class vector3;

//This is our 3x3 matrix class
class double3x3 {
public:
	double3x3();

	double3x3(double aa, double ab, double ac,
		double ba, double bb, double bc,
		double ca, double cb, double cc);

	double getij(int i, int j);
	
	double matrix[3][3] = { 0 };

	//multiplication between this matrix & another matrix
	double3x3 mmult(const double3x3& T);

	//multiply vector and this matrix (transform)
	vector3 vmult(const vector3& W);
};

//this is our vector 3 class. This has 1 additional property, w compared to the vector 2 class.
//This can represent both a vector and a point.
class vector3 {
public:
	vector3();
	vector3(const double x,const double y,const double w);
	double x, y, w;

	//return magnitude of this
	double mag();

	//dot this again vector b
	double dot(const vector3& b);
	//no operator looks like a dot so nevermind.

	//returns this-b
	vector3 minus(const vector3& b);

	//overload for convenience
	vector3 operator-(const vector3& b);

	double cross(const vector3& b);

	double operator*(const vector3& b);

	void norm();
};



double3x3 findW(vector3 A, vector3 B, vector3 Ap, vector3 Bp);
