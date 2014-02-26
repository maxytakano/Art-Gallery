#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <sstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Vector4L.hpp"

using namespace std;

Vector4L::Vector4L() {
    x = y = z = w = 0;
}

Vector4L::Vector4L(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1;
}

Vector4L::Vector4L(double x, double y, double z, double w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

// Some set's and get's
void Vector4L::set(double x, double y, double z, double w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

double Vector4L::get(int pos) {
    return this->operator[](pos);
}

double& Vector4L::operator[](int pos) {
    if (0 == pos) {
        return x;
    } else if (1 == pos) {
        return y;
    } else if (2 == pos) {
        return z;
    } else {
        return w;
    }
}

const double& Vector4L::operator[](int pos) const {
    if (0 == pos) {
        return x;
    } else if (1 == pos) {
        return y;
    } else if (2 == pos) {
        return z;
    } else {
        return w;
    }
}

// Addition
Vector4L Vector4L::add(Vector4L o) {
    return Vector4L(x + o.x, y + o.y, z + o.z, w + o.w);
}

Vector4L Vector4L::operator+(Vector4L o) {
    return Vector4L(x + o.x, y + o.y, z + o.z, w + o.w);
}

// Subtraction
Vector4L Vector4L::subtract(Vector4L o) {
    return Vector4L(x - o.x, y - o.y, z - o.z, w - o.w);
}

Vector4L Vector4L::operator-(Vector4L o) {
    return Vector4L(x - o.x, y - o.y, z - o.z, w - o.w);
}

void Vector4L::dehomogenize() {
    x = x / w;
    y = y / w;
    z = z / w;
    w = w / w;
}

string Vector4L::toString() {
  stringstream ret;

  ret << "Vector4L(";
  ret << x;
  ret << ", ";
  ret << y;
  ret << ", ";
  ret << z;
  ret << ", ";
  ret << w;
  ret << ")";

  return ret.str();
}
