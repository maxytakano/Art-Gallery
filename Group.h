#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"
#include <list>

using namespace std;

class Group: public Node {
public:
	void draw(Matrix4 c);
	void addChild(Node* n);
	void release();  

protected:
	list<Node*> childNodes;
	          // destructor

};


#endif
