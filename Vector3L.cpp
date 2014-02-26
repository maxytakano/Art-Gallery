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
#include "Vector3L.hpp"

Vector3L::Vector3L() {
  x = y = z = 0.0;
}

Vector3L::Vector3L(double x, double y, double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

void Vector3L::set(double x, double y, double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

double Vector3L::get(int pos) {
  return this->operator[](pos);
}

double& Vector3L::operator[](int pos) {
  if(0 == pos) {
    return this->x;
  } else if (1 == pos) {
    return this->y;
  } else {
    return this->z;
  }
}

const double& Vector3L::operator[](int pos) const {
  if(0 == pos) {
    return this->x;
  } else if (1 == pos) {
    return this->y;
  } else {
    return this->z;
  }
}

Vector3L Vector3L::add(Vector3L other) {
  return Vector3L(x + other.x, y + other.y, z + other.z);
}

Vector3L Vector3L::operator+(Vector3L other) {
  return Vector3L(x + other.x, y + other.y, z + other.z);
}

Vector3L Vector3L::subtract(Vector3L other) {
  return Vector3L(x - other.x, y - other.y, z - other.z);
}

Vector3L Vector3L::operator-(Vector3L other) {
  return Vector3L(x - other.x, y - other.y, z - other.z);
}

void Vector3L::negate() {
  x = -x;
  y = -y;
  z = -z;
}

void Vector3L::scale(double factor) {
  x = x * factor;
  y = y * factor;
  z = z * factor;
}

Vector3L Vector3L::operator*(float v) {
    return Vector3L(x * v, y * v, z * v);
}

double Vector3L::dotProduct(Vector3L other) {
  return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3L Vector3L::crossProduct(Vector3L other) {
  double nx = (y * other.z) - (z * other.y);
  double ny = (z * other.x) - (x * other.z);
  double nz = (x * other.y) - (y * other.x);

  return Vector3L(nx, ny, nz);
}

double Vector3L::magnitude() {
  return sqrt((x * x) + (y * y) + (z * z));
}

void Vector3L::normalize() {
  double mag = magnitude();

  double factor = 1.0 / mag;

  x = x * factor;
  y = y * factor;
  z = z * factor;
}

double Vector3L::angle(Vector3L& other) {
    return acos(this->dotProduct(other) / (this->magnitude() * other.magnitude()));
}

string Vector3L::toString() {
  stringstream ret;

  ret << "Vector3L(";
  ret << x;
  ret << ", ";
  ret << y;
  ret << ", ";
  ret << z;
  ret << ")";

  return ret.str();
}
