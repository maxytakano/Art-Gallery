#ifndef _MATRIX4L_H_
#define _MATRIX4L_H_

#include "Vector4L.hpp"
#include "Vector3L.hpp"

#include <string>
using namespace std;

class Matrix4L
{
  protected:
    double m[4][4];   // matrix elements
    
  public:
    Matrix4L();        // constructor
    Matrix4L(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
    double* getPointer();  // return pointer to matrix elements
    void identity();  // create identity matrix
    void bezier();
    void add(Matrix4L);
    void rotateX(double); // rotation about y axis
    void rotateY(double); // rotation about y axis
    void rotateZ(double); // rotation about y axis
    void rotateAxis(Vector3L, double); // rotation about y axis
    double get(int, int) const;
    void set(int, int, double);
    Matrix4L multiply(Matrix4L);
    Vector4L multiply(Vector4L);
    void scale(double, double, double);
    void translate(double, double, double);
    void transpose();
    string toString();
    string toStringRaw();
    Matrix4L* trackballRotation(int width, int height, int fromX, int fromY, int toX, int toY);
    void copyRot(Matrix4L*);
};

#endif
