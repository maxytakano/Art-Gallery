#ifndef _MODEL_H_
#define _MODEL_H_

#include "Matrix4.h" 
//#include "Matrix4.hpp" 

class Model
{
  protected:
    Matrix4 m;

    void update();
    
  public:
    Model();

    void identity();
    void rotateX(double);
    void rotateY(double);
    void rotateZ(double);
    void translate(double, double, double);
    void scale(double);
    void multiply(Matrix4);

    Matrix4 &getMatrix();
};

#endif
