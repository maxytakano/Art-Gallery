#include <math.h>
#include <iostream>

#include "Vector4.h"

Vector4::Vector4() {
	v[0] = 0.0;		// x
	v[1] = 0.0;		// y 		  
	v[2] = 0.0;		// z
	v[3] = 0.0;     // w      
}

Vector4::Vector4(float x0, float y0, float z0) {  // Constructor
	v[0] = x0;
	v[1] = y0;
	v[2] = z0;
}

Vector4::Vector4(float x0, float y0, float z0, float w0) {
	v[0] = x0;
	v[1] = y0;
	v[2] = z0;
	v[3] = w0;
}

void Vector4::set(float x0, float y0, float z0, float w0){ // Setter for vector coords
	v[0] = x0;
	v[1] = y0;
	v[2] = z0;
	v[3] = w0;
}

float Vector4::getX() {  // ??
	return v[0];
}

float Vector4::getY() {
	return v[1];
}

float Vector4::getZ() {
	return v[2];
}

float Vector4::getW() {
	return v[3];
}

void Vector4::setX(float x0) {
	v[0] = x0;
}
void Vector4::setY(float y0) {
	v[1] = y0;
}
void Vector4::setZ(float z0) {
	v[2] = z0;
}

float Vector4::operator[](int a) {  // TODO
	if (a < 0 || a > 3) {
		return v[0];
	}
	return v[a];
}

void Vector4::add(const Vector4 &a) {  // Vector addition
	v[0] = v[0] + a.v[0];
	v[1] = v[1] + a.v[1];
	v[2] = v[2] + a.v[2];
	v[3] = v[3] + a.v[3];
}

Vector4 Vector4::operator+(const Vector4& a) {
	Vector4 v2;

	v2.v[0] = a.v[0] + v[0];
	v2.v[1] = a.v[1] + v[1];
	v2.v[2] = a.v[2] + v[2];
	v2.v[3] = a.v[3] + v[3];

	return v2;
}

void Vector4::subtract(const Vector4 &a) {
	v[0] = v[0] - a.v[0];
	v[1] = v[1] - a.v[1];
	v[2] = v[2] - a.v[2];
	v[3] = v[3] - a.v[3];
}

Vector4 Vector4::operator-(const Vector4& a) {
	Vector4 v2;

	v2.v[0] = v[0] - a.v[0];
	v2.v[1] = v[1] - a.v[1];
	v2.v[2] = v[2] - a.v[2];
	v2.v[3] = v[3] - a.v[3];

	return v2;
}

void Vector4::Dehomogenize() {
	 v[3] = v[3] / v[3];
	 v[2] = v[2] / v[3];
	 v[1] = v[1] / v[3];
	 v[0] = v[0] / v[3];
}

void Vector4::print() {
	for(int i = 0; i < 4; i++) { 
			std::cout << v[i] << std::endl;
	}
}

float Vector4::magnitude() {
	return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

void Vector4::normalize() { 
	scale(1.0/magnitude());
}

void Vector4::scale(float s) {
	v[0] = v[0] * s;
	v[1] = v[1] * s;
	v[2] = v[2] * s;
}