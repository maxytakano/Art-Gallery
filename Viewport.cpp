#include "Viewport.hpp"

Viewport::Viewport(int oX0, int oX1, int oY0, int oY1) {
  px0 = oX0;
  px1 = oX1;
  py0 = oY0;
  py1 = oY1;

  update();
}

void Viewport::update() {
  m = Matrix4();

  m.set(0, 0, (x1() - x0()) / 2.0);
  m.set(0, 3, (x0() + x1()) / 2.0);

  m.set(1, 1, -(y1() - y0()) / 2.0);
  m.set(1, 3, (y0() + y1()) / 2.0);

  m.set(2, 2, -0.5);
  m.set(2, 3, 0.5);

  m.set(3, 3, 1);
}

int Viewport::x0() const {
  return px0;
}

int Viewport::x1() const {
  return px1;
}

int Viewport::y0() const {
  return py0;
}

int Viewport::y1() const {
  return py1;
}

void Viewport::x0(int n) {
  px0 = n;
  update();
}

void Viewport::x1(int n) {
  px1 = n;
  update();
}

void Viewport::y0(int n) {
  py0 = n;
  update();
}

void Viewport::y1(int n) {
  py1 = n;
  update();
}

Matrix4 &Viewport::getMatrix() {
  return m;
}
