#ifndef _VECTOR4_H_
#define _VECTOR4_H_

class Vector4 {
//private:
	//float v[3];
public: 
	float v[4];
	Vector4();
	Vector4(float x0, float y0, float z0);  // Constructor
	Vector4(float x0, float y0, float z0, float w0);
	void set(float x0, float y0, float z0, float w0); // Setter for vector coords
	float getX();  // ??
	float getY();
	float getZ();
	float getW();
	void setX(float x0);
	void setY(float y0);
	void setZ(float z0);

	float operator[](int a);
	void add(const Vector4 &a);  // Vector addition
	Vector4 operator+(const Vector4& a);
	void subtract(const Vector4 &a);
	Vector4 operator-(const Vector4& a);
	void Dehomogenize();
	void print();
	float magnitude();
	void normalize();
	void scale(float s);

};

#endif
