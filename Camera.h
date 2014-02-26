#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Matrix4.h"
#include "Vector3.h"

class Camera {
private:
	Vector3 up;
	Vector3 d;
	Vector3 e;
	Matrix4 c;

	Vector3 zc, xc, yc;

public:
	Camera();
	
	Camera(Vector3 up, Vector3 d, Vector3 e);
	void initializeMatrix();
	Matrix4 getCameraMatrix();
	void set(Vector3 up1, Vector3 d1, Vector3 e1);
};

#endif
