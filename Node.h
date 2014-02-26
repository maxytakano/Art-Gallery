#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Matrix4.h"

class Node {
public:
	virtual void draw(Matrix4 c) = 0;
  virtual ~Node() {};
	

protected:
	int nodeID;		// node's id
	Node *parent;	// pointer to parent

};


#endif
