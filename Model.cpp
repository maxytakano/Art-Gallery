#include "Model.hpp"

Model::Model() {
    m = Matrix4();
    m.identity();
}

void Model::update() {
}

void Model::identity() {
  m.identity();
}

void Model::rotateX(double degree) {
    Matrix4 r = Matrix4();
    r.rotateX(degree);

    m = r.multiply(m);
}

void Model::rotateY(double degree) {
    Matrix4 r = Matrix4();
    r.rotateY(degree);

    m = r.multiply(m);
}

void Model::rotateZ(double degree) {
    Matrix4 r = Matrix4();
    r.rotateZ(degree);

    m = r.multiply(m);
}

void Model::translate(double x, double y, double z) {
    Matrix4 t = Matrix4();
    t.translate(x, y, z);

    m = t.multiply(m);
}

void Model::scale(double scale) {
    Matrix4 s = Matrix4();
    s.scale(scale, scale, scale);

    m = s.multiply(m);
}

void Model::multiply(Matrix4 o) {
  m = o.multiply(m);
}

Matrix4 &Model::getMatrix() {
    return m;
}
