#include "MatrixTransform.h"

MatrixTransform::MatrixTransform() {
	M.identity();
}

MatrixTransform::MatrixTransform(Matrix4 t) {
	M = t;           // set matrix = to the rotation matrix
}

MatrixTransform::MatrixTransform(Matrix4 t, float x1, float y1, float z1) {
	M = t;           // set matrix = to the rotation matrix
	x = x1;
	y = y1;
	z = z1;

	pls.set(x, y, z);

}

void MatrixTransform::draw(Matrix4 c) {
	Matrix4 C_new;
	C_new = c.multiply(M); // M is a class member
	for (std::list<Node*>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {
		(*it)->draw(C_new);
	}
	
}

void MatrixTransform::addChild(Node* n) {
	childNodes.push_back(n);
}

void MatrixTransform::release() {
	for (std::list<Node*>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {
		delete *it;
	}
}

Matrix4& MatrixTransform::getMatrix()
{
  return M;
}


void MatrixTransform::update(Matrix4 u) {
	M = u;
}
