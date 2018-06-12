#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>


typedef struct{
    double x,y,w;
} vector3;

typedef struct{
    double entries[9];   //3 x 3 matrix
} matrix33;

vector3 vector3_applymatrix(vector3 *vector, matrix33 *matrix){
    // dot product
    vector3 temp;
    double *entries = matrix->entries;
    temp.x = vector->x * entries[0] + vector->y * entries[1] + vector->w * entries[2];
    temp.y = vector->x * entries[3] + vector->y * entries[4] + vector->w * entries[5];  //column 2
    temp.w = vector->x * entries[6] + vector->y * entries[7] + vector->w * entries[8];  //column 3
    return temp;
}

vector3 vector3_minus(vector3 *a, vector3 *b){
    //returns vector a - b 
    vector3 temp;
    temp.x = a->x - b->x;
    temp.y = a->y - b->y;
    temp.w = a->w - b->w;
    return temp;
}

double vector3_dot(vector3 *a, vector3 *b){
    return b->x*a->x+b->y*a->y+b->w*a->w;
}

vector3 makevector(double x, double y, double w){
    vector3 temp;
    temp.x = x; temp.y = y; temp.w = w;
    return temp;
}

double vector3_magnitude(vector3 *vector){
    double x = vector->x;
    double y = vector->y;
    return sqrt(x*x + y*y);
}

void vector3_normalize(vector3 * vector){
    double mag = vector3_magnitude(vector);
    vector->x /= mag;
    vector->y /= mag;
}
double matrix33_getentry(matrix33* matrix, int i, int j){
    //get the ith row jth column entry
    return matrix->entries[3*i + j -4];
}

void matrix33_setentry(matrix33* matrix, int i, int j, double v){
    //get the ith row jth column entry
    matrix->entries[3*i + j -4] = v;
}

matrix33 makematrixFromArray(double entries[]){
    matrix33 temp;
    double *entry = temp.entries;
    for(int i=0;i<9;i++){
        entry[i] = entries[i];
    }
    return temp;
}

matrix33 makematrix(double aa, double ab, double ac, double ba, double bb, double bc, double ca, double cb, double cc){
    double entries[] = {aa,ab,ac,
                       ba,bb,bc,
                       ca,cb,cc};
    return makematrixFromArray(entries);
}

matrix33 matrix33_random(){
    double a[9];
    srand(time(NULL));
    for (int i=0;i<9;i++){
        a[i] = (float)rand()/(float)(RAND_MAX/100); //[0,100]
    }
    return makematrixFromArray(a);
}

matrix33 matrix33_multiply(matrix33 *a, matrix33 *b){
    matrix33 temp;
    //c_ij = a_i1 b_1j + ....
    for (int i = 1; i < 4; i++) {
        vector3 vR = makevector(matrix33_getentry(a,i,1),matrix33_getentry(a,i,2),matrix33_getentry(a,i,3));
		for (int j = 1; j < 4; j++) {
			vector3 vT = makevector(matrix33_getentry(b,1,j),matrix33_getentry(b,2,j),matrix33_getentry(b,3,j));
			matrix33_setentry(&temp,i,j,vector3_dot(&vR,&vT));
		}
	}
	return temp;
}

void print_matrix33(matrix33 *matrix){
    for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			printf("%f ", matrix33_getentry(matrix,i,j));
		}
		printf("\n");
	}
	printf("\n");
}

matrix33 findW(vector3 *A, vector3 *B, vector3 *Ap, vector3 *Bp){
    /*
    1. Rotate d so it is parallel to dp
    2. Scale to fit
    3. Translate d onto dp
    */
    matrix33 W;
    vector3 d, dp, dptemp;
    
    d = vector3_minus(A,B);
    dp = vector3_minus(Ap,Bp);
    dptemp = dp;
    vector3_normalize(&d);
    vector3_normalize(&dptemp);
    
    //d dot dp
	double costheta = d.x*dptemp.x + d.y*dptemp.y;
	// d cross dp
	double sintheta = d.x*dptemp.y - dptemp.x*d.y;
	
	if (isnan(costheta))costheta = 1;
	if (isnan(sintheta))sintheta = 0;
	
	matrix33 R = makematrix(costheta,   -sintheta,  0.0,
	                        sintheta,   costheta,   0.0,
	                        0.0,        0.0,        1.0);
    printf("Rotation matrix:\n");
    print_matrix33(&R);
      
     
	vector3 AR = vector3_applymatrix(A, &R);
	vector3 BR = vector3_applymatrix(B, &R);
	
	//now d = s dP where s is a scalar.
	d = vector3_minus(&AR,&BR);
	
	//scaling wrt different axes
	double Sx = dp.x / d.x;
	double Sy = dp.y / d.y;
	if (isnan(Sx))Sx = 1;
	if (isnan(Sy))Sy = 1;
	
	
	matrix33 S = makematrix(Sx, 0, 0,
							0, Sy, 0,
							0, 0, 1);
	printf("Scale matrix:\n");
    print_matrix33(&S);
							
	vector3 ARS = vector3_applymatrix(&AR, &S);
	
	double Tx = Ap->x - ARS.x;
	double Ty = Ap->y - ARS.y;

	matrix33 T = makematrix(1.0, 0.0, Tx,
							0.0, 1.0, Ty,
							0.0, 0.0, 1.0);
							
	printf("Translation matrix:\n");
    print_matrix33(&T);
    
	matrix33 RS = matrix33_multiply(&R, &S);
	W = matrix33_multiply(&T, &RS);
	
	printf("Overall calculated transform matrix:\n");
	print_matrix33(&W);
	
	return W;
}

int main()
{
    double epsilon = 0.0000001f;
    /* W is world transform
       R is rotation
       S is scaling
       T is translation 
       
       The points before transformation are A and before
       After is Ap and Bp ( p for prime )
       The line segment AB is d
       The line segment ApBp is dp
    */
    vector3 A, Ap, B, Bp;
    matrix33 genW;  // random world transform
    genW = matrix33_random();
    
    printf("Input A.x ");
    scanf("%lf", &A.x);
    printf("Input A.y ");
    scanf("%lf", &A.y);
    printf("Input B.x ");
    scanf("%lf", &B.x);
    printf("Input B.y ");
    scanf("%lf", &B.x);
    A.w = 1; B.w= 1;
    
    printf("Randomly generated transform matrix:\n");
    print_matrix33(&genW);
    
    Ap = vector3_applymatrix(&A,&genW);
    Bp = vector3_applymatrix(&B,&genW);
    
    printf("Generated matrix on A: x: %f \ty: %f\n", Ap.x, Ap.y);
    printf("Generated matrix on B: x: %f \ty: %f\n\n", Bp.x, Bp.y);
    
    matrix33 W = findW(&A,&B,&Ap,&Bp);
    vector3 AW = vector3_applymatrix(&A, &W);
    vector3 BW = vector3_applymatrix(&B, &W);
    
    printf("Calculated matrix on A: x: %f y: %f\n", AW.x, AW.y);
    printf("Calculated matrix on B: x: %f y: %f\n\n", BW.x, BW.y);
    
    vector3 temp = vector3_minus(&AW, &Ap);
	if (vector3_magnitude(&temp)< epsilon) printf("Research has shown that the matrix applied on A is accurate enough!\n");
	else printf("Something went wrong with A \n");
	temp = vector3_minus(&BW,&Bp);
	if (vector3_magnitude(&temp)< epsilon) printf("Research has shown that the matrix applied on B is accurate enough!\n");
	else printf("Something went wrong with B \n");

    return 0;
}
