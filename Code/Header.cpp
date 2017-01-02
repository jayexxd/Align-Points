#include "Header.h"

vector3::vector3() :
x(0),y(0),w(1){}

vector3::vector3(const double x, const double y, const double w):
	x(x),y(y),w(w)	//magic
{}

double vector3::mag()
{
	//return the magnitude of the point
	//we dont really want to include w
	return sqrt(this->x*this->x+this->y*this->y);
}

double vector3::dot(const vector3& b)
{
	return b.x*this->x+b.y*this->y+b.w*this->w;
}

vector3 vector3::minus(const vector3& b)
{
	vector3 temp = vector3::vector3();
	temp.x = this->x - b.x;
	temp.y = this->y - b.y;
	temp.w = this->w - b.w;
	return temp;
}

vector3 vector3::operator-(const vector3& b)
{
	return minus(b);
}
//note this is the 2D cross because our vector is a 
//homogeneous vector and not an actual 3D vector.
double vector3::cross(const vector3 & b)
{
	return this->x*b.y-this->y*b.x;
}

double vector3::operator*(const vector3 & b)
{
	return cross(b);
}

void vector3::norm()
{
	double magnitude = this->mag();
	this->x /= magnitude;
	this->y /= magnitude;
}

double3x3::double3x3() :
	matrix{0}	//c++11
{}

double3x3::double3x3(double aa, double ab, double ac, double ba, double bb, double bc, double ca, double cb, double cc) :
	matrix { aa,ab,ac,ba,bb,bc,ca,cb,cc }	//c++11
{}

double double3x3::getij(int i, int j)
{
	return matrix[++i][++j];	
}

double3x3 double3x3::mmult(const double3x3 & T)
{
	double3x3 temp = double3x3();
	//We use this definition of matrix multiplication because it is clearer and easier to debug.
	//it might be slightly slower.
	for (int i = 0; i < 3; i++) {
		vector3 vR(this->matrix[i][0], this->matrix[i][1], this->matrix[i][2]);
		for (int j = 0; j < 3; j++) {
			vector3 vT(T.matrix[0][j], T.matrix[1][j], T.matrix[2][j]);
			temp.matrix[i][j] = vR.dot(vT);
		}
	}
	return temp;
}


//same as above, but now its scalar dot vector so we can just write it down.
vector3 double3x3::vmult(const vector3 & W)
{
	vector3 temp = vector3();
	temp.x = W.x*this->matrix[0][0] + W.y*this->matrix[1][0] + W.w*this->matrix[2][0];
	temp.y = W.x*this->matrix[0][1] + W.y*this->matrix[1][1] + W.w*this->matrix[2][1];
	temp.w = W.x*this->matrix[0][2] + W.y*this->matrix[1][2] + W.w*this->matrix[2][2];

	return temp;
}


/*Proposed solution:
1. Rotate d so it is parallel to dp
2. scale them so they are the same size
3. translate d onto dp.
*/
double3x3 findW(vector3 A, vector3 B, vector3 Ap, vector3 Bp) {
	double3x3 W = double3x3();
	vector3 d, dp = vector3();
	//the 2 lines segments AB and ApBp
	d = A - B; dp = Ap - Bp;
	//we need a temp because we are gonna normalize everything
	//we need the original to know how much to scale by.
	vector3 dptemp = dp;
	d.norm(); dptemp.norm();

	//also d DOT dp
	double costheta = d.x*dptemp.x + d.y*dptemp.y;
	//also magitude of d CROSS dp
	double sintheta = d.x*dptemp.y - dptemp.x*d.y;

	if (isnan(costheta))costheta = 1;
	if (isnan(sintheta))sintheta = 0;

	double3x3 R = double3x3(costheta, -sintheta, 0.0,
							sintheta, costheta, 0.0,
							0.0, 0.0, 1.0);

	//we rotate it now
	vector3 AR = R.vmult(A);
	vector3 BR = R.vmult(B);

	//now d = s dP where s is a scalar.
	d = AR - BR;

	//scaling wrt different axes
	double Sx = dp.x / d.x;
	double Sy = dp.y / d.y;
	if (isnan(Sx))Sx = 1;
	if (isnan(Sy))Sy = 1;

	double3x3 S = double3x3(Sx, 0, 0,
							0, Sy, 0,
							0, 0, 1);
	//scale it now
	vector3 ARS = S.vmult(AR);

	//translate it to align
	double Tx = Ap.x - ARS.x;
	double Ty = Ap.y - ARS.y;

	double3x3 T = double3x3(1.0, 0.0, 0.0,
							0.0, 1.0, 0.0,
							Tx, Ty, 1.0);

	//for you to check the numbers at the end.

	W = R.mmult(S.mmult(T));
	return W;
}
