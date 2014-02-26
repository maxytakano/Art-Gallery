#ifndef _BENCH_H_
#define _BENCH_H_

#include "Node.h"

class Bench: public Node {
public:
	Bench();
	Bench(float r1, float g1, float b1);
	void draw(Matrix4 c);

protected:
	float r, g, b;

};


#endif
