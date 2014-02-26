#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "Group.h"

class MatrixTransform: public Group {
public:
	MatrixTransform();
	MatrixTransform(Matrix4 t);
	MatrixTransform(Matrix4 t, float x, float y, float z);
	void draw(Matrix4 c);
	void addChild(Node* n);
	void release();            // destructor
	Matrix4& getMatrix();
	void update(Matrix4 u);
	
	list<Node*> childNodes;

protected:
	Matrix4 M;
	float x, y, z;

	Vector3 pls; 
	
};


#endif