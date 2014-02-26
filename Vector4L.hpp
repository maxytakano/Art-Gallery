#ifndef _VECTOR4L_H_
#define _VECTOR4L_H_

#include <string>
#include "Vector3L.hpp"

using namespace std;

class Vector4L
{
  protected:
      double x;
      double y;
      double z;
      double w;

  public:
      Vector4L();
      Vector4L(double, double, double);
      Vector4L(double, double, double, double);

      // Some set's and get's
      void set(double, double, double, double);
      double get(int);
      double& operator[](int);
      const double& operator[](int) const;

      // Addition
      Vector4L add(Vector4L);
      Vector4L operator+(Vector4L);

      // Subtraction
      Vector4L subtract(Vector4L);
      Vector4L operator-(Vector4L);

      void dehomogenize();

      string toString();
};

#endif
