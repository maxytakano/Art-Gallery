#ifndef _VECTOR3_H_
#define _VECTOR3_H_

class Vector3 {

public: 
	float v[3];
	Vector3();
	Vector3(float x0, float y0, float z0);  // Constructor
	void set(float x0, float y0, float z0); // Setter for vector coords
	float getX(); 
	float getY();
	float getZ();
	void setX(float x0);
	void setY(float y0);
	void setZ(float z0);
	float operator[](int a);
	Vector3 add(const Vector3 &a);  // Vector addition
	Vector3 operator+(const Vector3& a);
	Vector3 subtract(const Vector3 &a);
	Vector3 operator-(const Vector3& a);
	void negate();		   // Negate vector
	void scale(float s);
	float dot(Vector3 &a);
	Vector3 cross(Vector3 &a, Vector3 &b);
	float magnitude();
	void normalize();
	void print(); // ?? 


};

#endif
