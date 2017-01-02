#include "Header.h"
#include <random>
#include <time.h>
#include<iostream>
#include<iomanip>

int main() {
	double epsilon = 0.0001;	//how much floating point error you are willing to accomodate

	/*W is the world transform.
		S is scaling
		R is rotation
		T is translate
	*/

	/*The points before transformation are A and B
	After is Ap and Bp (p for prime)
	The line segment AB is d
	ApBp is dp
	*/
	vector3 A, Ap, B, Bp = vector3();

	std::cout << "Input A\n";
	std::cin >> A.x >> A.y;
	std::cout << "Input B\n";
	std::cin >> B.x >> B.y;

	std::mt19937 rand(time(NULL));
	double3x3 genW = double3x3(rand() % 100, rand() % 100, rand() % 100,
							   rand() % 100, rand() % 100, rand() % 100,
							   rand() % 100, rand() % 100, rand() % 100);
	Ap = genW.vmult(A);
	Bp = genW.vmult(B);
	/*
	std::cout << "Input A prime\n";
	std::cin >> Ap.x >> Ap.y;
	std::cout << "Input B prime\n";
	std::cin >> Bp.x >> Bp.y;
	std::cout << std::endl;
	*/

	double3x3 W = findW(A, B, Ap, Bp);
	vector3 AW = W.vmult(A);
	vector3 BW = W.vmult(B);
	std::cout << std::left<<std::setprecision(5);
	std::cout << "This is the value of A times generated matrix " << std::setw(10) << Ap.x << ", " << std::setw(20) << Ap.y << std::endl;
	std::cout << "This is the value of B times generated matrix " << std::setw(10) << Bp.x << ", " << std::setw(20) << Bp.y << std::endl;


	std::cout << "My calculated matrix applied on A gives: " << std::setw(10) << AW.x << ", " << std::setw(20) << AW.y << std::endl;
	std::cout << "My calculated matrix applied on B gives: " << std::setw(10) << BW.x << ", " << std::setw(20) << BW.y << std::endl;

	vector3 temp = AW - Ap;
	if (temp.mag() < epsilon) std::cout << "My tests show that the matrix on A is accurate enough!\n";
	else std::cout << "Something went wrong with A \n";
	temp = BW - Bp;
	if (temp.mag() < epsilon) std::cout << "My tests show that the matrix on B is accurate enough!\n";
	else std::cout << "Something went wrong with B \n";
	
	std::cout << "This are the generated values of W: \n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << std::setw(20) << genW.matrix[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << "This are the calculated values of W: \n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << std::setw(20) << W.matrix[i][j];
		}
		std::cout << std::endl;
	}


	//just need to pause the program...
	std::cin >> epsilon;
	return 0;
}
