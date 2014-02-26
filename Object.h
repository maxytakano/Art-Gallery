#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Node.h"
#include <vector>
#include "tiny_obj_loader.h"

using namespace tinyobj;
using namespace std;

class Object: public Node {
public:
	Object();
	Object(float r1, float g1, float b1);
	void draw(Matrix4 c);

protected:

	float r, g, b;
  static bool loaded;
  static vector<shape_t> data;
  void loadData();

  virtual char* getName() = 0;
  virtual bool* getLoaded() = 0;
  virtual vector<shape_t>* getData() = 0;

};


#endif
