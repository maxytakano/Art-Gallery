#include "Group.h"

void Group::draw(Matrix4 C) {
	for (std::list<Node*>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {
		(*it)->draw(C);
	}
}

void Group::addChild(Node* n) {
	childNodes.push_back(n);
}

void Group::release() {
	for (std::list<Node*>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {
		delete *it;
	}
}
