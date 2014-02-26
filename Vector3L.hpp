#ifndef _VECTOR3L_H_
#define _VECTOR3L_H_

#include <string>
#include "Vector4L.hpp"

using namespace std;

class Vector3L
{
  protected:
      double x;
      double y;
      double z;

  public:
      Vector3L();
      Vector3L(double, double, double);

      // Some set's and get's
      void set(double, double, double);
      double get(int);
      double& operator[](int);
      const double& operator[](int) const;

      // Addition
      Vector3L add(Vector3L);
      Vector3L operator+(Vector3L);

      // Subtraction
      Vector3L subtract(Vector3L);
      Vector3L operator-(Vector3L);

      void negate();

      void scale(double);
      Vector3L operator*(float);

      double dotProduct(Vector3L);

      Vector3L crossProduct(Vector3L);

      double magnitude();

      double angle(Vector3L&);

      void normalize();

      string toString();
};

#endif
