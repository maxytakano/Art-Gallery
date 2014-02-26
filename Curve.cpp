#include "Curve.h"
#include <iostream>

using namespace std;

void Curve::calculateG() {
  Matrix4L tmp = Matrix4L();
  Vector3L* vs[] = {p0, p1, p2, p3};
  for(int i = 0; i < 4; i++) {
    tmp.set(0, i, vs[i]->get(0));
    tmp.set(1, i, vs[i]->get(1));
    tmp.set(2, i, vs[i]->get(2));
    tmp.set(3, i, 1);
  }

  Matrix4L b = Matrix4L();
  b.bezier();
  g = tmp.multiply(b);
}

Vector4L Curve::point(double t) {
  Vector4L ts = Vector4L(t*t*t, t*t, t, 1);
  return g.multiply(ts);
}

Vector3L Curve::pointNormal(double t) {
  Vector4L ts = Vector4L(3*t*t, 2*t, 1, 0);
  Vector4L res = g.multiply(ts);
  Vector3L tst = Vector3L(res[0], res[1], res[2]);
  return tst;
}
