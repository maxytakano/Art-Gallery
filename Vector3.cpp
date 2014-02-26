#include <math.h>
#include <iostream>

#include "Vector3.h"

Vector3::Vector3() {
	v[0] = 0.0;		// x
	v[1] = 0.0;		// y 		  
	v[2] = 0.0;		// z
}

Vector3::Vector3(float x0, float y0, float z0) {  // Constructor
	v[0] = x0;
	v[1] = y0;
	v[2] = z0;
}

void Vector3::set(float x0, float y0, float z0) { // Setter for vector coords
	v[0] = x0;
	v[1] = y0;
	v[2] = z0;
}

void Vector3::setX(float x0) {
	v[0] = x0;
}

void Vector3::setY(float y0) {
	v[1] = y0;
}

void Vector3::setZ(float z0) {
	v[2] = z0;
}

float Vector3::getX() {
	return v[0];
}

float Vector3::getY() {
	return v[1];
}

float Vector3::getZ() {
	return v[2]; 
}

float Vector3::operator[](int a) {     // TODO
	if (a < 0 || a > 2) {
		return v[0];
	}
	return v[a];
}

Vector3 Vector3::add(const Vector3& a) {			// Vector addition
	Vector3 v2;

	v2.v[0] = v[0] + a.v[0];
	v2.v[1] = v[1] + a.v[1];
	v2.v[2] = v[2] + a.v[2];

	return v2;
}

Vector3 Vector3::operator+(const Vector3& a) {
	Vector3 v2;

	v2.v[0] = a.v[0] + v[0];
	v2.v[1] = a.v[1] + v[1];
	v2.v[2] = a.v[2] + v[2];

	return v2;
}

Vector3 Vector3::subtract(const Vector3 &a) {
	Vector3 v2;

	v2.v[0] = v[0] - a.v[0];
	v2.v[1] = v[1] - a.v[1];
	v2.v[2] = v[2] - a.v[2];

	return v2;
}

Vector3 Vector3::operator-(const Vector3& a) {
	Vector3 v2;

	v2.v[0] = v[0] - a.v[0];
	v2.v[1] = v[1] - a.v[1];
	v2.v[2] = v[2] - a.v[2];

	return v2;
}

void Vector3::negate() {		   // Negate vector
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void Vector3::scale(float s) {
	v[0] = v[0] * s;
	v[1] = v[1] * s;
	v[2] = v[2] * s;
}

float Vector3::dot(Vector3 &a) {
	return v[0]*a.v[0]+v[1]*a.v[1]+v[2]*a.v[2];
}

Vector3 Vector3::cross(Vector3 &a, Vector3 &b) {
	Vector3 v2;

	v2.v[0]=a.v[1]*b.v[2]-a.v[2]*b.v[1]; 
	v2.v[1]=a.v[2]*b.v[0]-a.v[0]*b.v[2];
	v2.v[2]=a.v[0]*b.v[1]-a.v[1]*b.v[0];

	return v2; 
}

float Vector3::magnitude() {
	return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

void Vector3::normalize() { 
	scale(1.0/magnitude());
}

void Vector3::print() { 
	std::cout << '<' << v[0] << ',' << v[1] << ',' << v[2] << '>' << std::endl;
}

