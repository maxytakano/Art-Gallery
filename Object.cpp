#include "Object.h"

void Object::loadData() {
  if(!*getLoaded()) {
    vector<shape_t> d;
    LoadObj(d, getName(), NULL);
    *(getData()) = d;
    *getLoaded() = true;
  }
}

Object::Object() {
	r = 0;
	g = 1;
	b = 0;
}

Object::Object(float r1, float g1, float b1) {
	r = r1;
	g = g1;
	b = b1;
}

void Object::draw(Matrix4 c) {
    loadData();
		glColor3f(r, g, b);

		glLoadMatrixd(c.transpose().getPointer());

    for(vector<shape_t>::iterator it = (*getData()).begin() ; it != (*getData()).end(); ++it) {
      shape_t shape = *it;

      glEnableClientState( GL_VERTEX_ARRAY );
      glEnableClientState( GL_NORMAL_ARRAY );
      glVertexPointer(3, GL_FLOAT, 0, shape.mesh.positions.data() );
      glNormalPointer(GL_FLOAT, 0, shape.mesh.normals.data() );
      glDrawElements( GL_TRIANGLES, shape.mesh.indices.size(), GL_UNSIGNED_INT, shape.mesh.indices.data() );
    }
}
