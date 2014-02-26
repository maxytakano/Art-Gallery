#include "Camera.h"

Camera::Camera()
{
	Camera::Camera(Vector3(0,0,0),Vector3(0,0,0),Vector3(0,0,0));
}

Camera::Camera(Vector3 up, Vector3 d, Vector3 e) {
	this->up = up;
	this->d = d;
	this->e = e;
	
	initializeMatrix();
}

void Camera::set(Vector3 up1, Vector3 d1, Vector3 e1) {
	up = up1;
	d = d1;
	e = e1;

	initializeMatrix();
}

void Camera::initializeMatrix() {
	Vector3 temp;

	zc =  ( e.subtract(d) );
	zc.normalize();

	xc = ( temp.cross(up, zc) );
	xc.normalize();

	yc = temp.cross(zc, xc);

	//xc.print();
	//yc.print();
	//zc.print();
	
	
	// initialize the matrix with zc, xc, and yc [xc, yc, zc, e]
	Matrix4 translation(1, 0, 0, -e.getX(),
						0, 1, 0, -e.getY(),
						0, 0, 1, -e.getZ(),
						0, 0, 0, 1);
	translation = translation.transpose();
	Matrix4 rotation(xc.getX(), yc.getX(), zc.getX(), 0, 
					  xc.getY(), yc.getY(), zc.getY(), 0,         
					  xc.getZ(), yc.getZ(), zc.getZ(), 0, 
					  0,  0,  0,  1);
	
	c = translation.multiply(rotation);
	c = c.transpose();
		
}

Matrix4 Camera::getCameraMatrix() {
	return c;
}
