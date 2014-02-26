#ifndef _MATRIX4_H_
#define _MATRIX4_H_

//#include "C:\Users\Max\Documents\Visual Studio 2010\Projects\Lab1\Lab1\Vector4.h";
//#include "C:\Users\Max\Documents\Visual Studio 2010\Projects\Lab1\Lab1\Vector3.h";
#include "Vector4.h"
#include "Vector3.h"

class Matrix4
{
  protected:
    double m[4][4];   // matrix elements
	double oned[16];
	double invOut[16];
	double inv[16];
    
  public:
    Matrix4();        // constructor
    Matrix4(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
    double* getPointer();  // return pointer to matrix elements
    void identity();  // create identity matrix
    void rotateY(double); // rotation about y axis

	int get(int x,int y);
	Matrix4 multiply(const Matrix4& m2);
	Vector4 multiply(const Vector4& a);
	void rotateX(double angle);
	Matrix4 rotateZ(double angle);
	Matrix4 rotateArbitrary(double radians, double x, double y, double z); // ??
	Matrix4 scaling(double s, double t, double r); 
	Matrix4 translation(double t1, double t2, double t3);
	void printMatrix(); 
	Matrix4 transpose(); 
	void set(double m00, double m10, double m20, double m30,
	  double m01, double m11, double m21, double m31,
	  double m02, double m12, double m22, double m32,
	  double m03, double m13, double m23, double m33);
	double* getValues();
	void printoned();
	void invert();
	void copyRot(Matrix4 r);

	Matrix4 trackballRotation(int width, int height, int fromX, int fromY, int toX, int toY);

};

#endif
