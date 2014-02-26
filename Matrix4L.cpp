#include <math.h>
#include <sstream>
#include <iostream>

#include "Matrix4L.hpp"

using namespace std;

double matrixSubMult(const Matrix4L* m, const Matrix4L* o, int row, int col);
double matrixVectorSubMult(const Matrix4L* m, int row, Vector4L o);

Matrix4L::Matrix4L()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
}
Matrix4L::Matrix4L(
  double m00, double m01, double m02, double m03,
  double m10, double m11, double m12, double m13,
  double m20, double m21, double m22, double m23,
  double m30, double m31, double m32, double m33 )
{
  this->set(0, 0, m00);
  this->set(0, 1, m01);
  this->set(0, 2, m02);
  this->set(0, 3, m03);
  this->set(1, 0, m10);
  this->set(1, 1, m11);
  this->set(1, 2, m12);
  this->set(1, 3, m13);
  this->set(2, 0, m20);
  this->set(2, 1, m21);
  this->set(2, 2, m22);
  this->set(2, 3, m23);
  this->set(3, 0, m30);
  this->set(3, 1, m31);
  this->set(3, 2, m32);
  this->set(3, 3, m33);
}

double* Matrix4L::getPointer()
{
  return &m[0][0];
}

void Matrix4L::identity()
{
  double ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = ident[i][j];
    }
  }
}

void Matrix4L::bezier() {
    this->identity();
    this->set(0,0, 1);
    this->set(1,0, -3);
    this->set(1,1, 3);
    this->set(2,0, 3);
    this->set(2,1, -6);
    this->set(2,2, 3);
    this->set(3,0, -1);
    this->set(3,1, 3);
    this->set(3,2, -3);
    this->set(3,3, 1);
}

void Matrix4L::add(Matrix4L o) {
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      this->set(i, j, this->get(i, j) + o.get(i, j));
    }
  }
}

// angle in radians
void Matrix4L::rotateX(double angle)
{
	this->set(0, 0, 1);
	this->set(0, 1, 0);
	this->set(0, 2, 0);
	this->set(0, 3, 0);
	this->set(1, 0, 0);
	this->set(1, 1, cos(angle));
	this->set(1, 2, -sin(angle));
	this->set(1, 3, 0);
	this->set(2, 0, 0);
	this->set(2, 1, sin(angle));
	this->set(2, 2, cos(angle));
	this->set(2, 3, 0);
	this->set(3, 0, 0);
	this->set(3, 1, 0);
	this->set(3, 2, 0);
	this->set(3, 3, 1);
}

// angle in radians
void Matrix4L::rotateY(double angle)
{
    this->set(0, 0, cos(angle));
    this->set(0, 1, 0);
    this->set(0, 2, sin(angle));
    this->set(0, 3, 0);
    this->set(1, 0, 0);
    this->set(1, 1, 1);
    this->set(1, 2, 0);
    this->set(1, 3, 0);
    this->set(2, 0, -sin(angle));
    this->set(2, 1, 0);
    this->set(2, 2, cos(angle));
    this->set(2, 3, 0);
    this->set(3, 0, 0);
    this->set(3, 1, 0);
    this->set(3, 2, 0);
    this->set(3, 3, 1);
}

// angle in radians
void Matrix4L::rotateZ(double angle)
{
    this->set(0, 0, cos(angle));
    this->set(0, 1, -sin(angle));
    this->set(0, 2, 0);
    this->set(0, 3, 0);
    this->set(1, 0, sin(angle));
    this->set(1, 1, cos(angle));
    this->set(1, 2, 0);
    this->set(1, 3, 0);
    this->set(2, 0, 0);
    this->set(2, 1, 0);
    this->set(2, 2, 1);
    this->set(2, 3, 0);
    this->set(3, 0, 0);
    this->set(3, 1, 0);
    this->set(3, 2, 0);
    this->set(3, 3, 1);
}

// angle in radians
void Matrix4L::rotateAxis(Vector3L axis, double angle)
{
    double x = axis[0];
    double y = axis[1];
    double z = axis[2];
    this->set(0, 0, cos(angle) + (x * x * (1 - cos(angle))));
    this->set(0, 1, (x * y * (1 - cos(angle))) - (z * sin(angle)));
    this->set(0, 2, (x * z * (1 - cos(angle))) + (y * sin(angle)));
    this->set(0, 3, 0);
    this->set(1, 0, (y * x * (1 - cos(angle))) + (z * sin(angle)));
    this->set(1, 1, cos(angle) + (y * y * (1 - cos(angle))));
    this->set(1, 2, (y * z * (1 - cos(angle))) - (x * sin(angle)));
    this->set(1, 3, 0);
    this->set(2, 0, (z * x * (1 - cos(angle))) - (y * sin(angle)));
    this->set(2, 1, (z * y * (1 - cos(angle))) + (x * sin(angle)));
    this->set(2, 2, cos(angle) + (z * z * (1 - cos(angle))));
    this->set(2, 3, 0);
    this->set(3, 0, 0);
    this->set(3, 1, 0);
    this->set(3, 2, 0);
    this->set(3, 3, 1);
}

double Matrix4L::get(int i, int j) const {
  return m[j][i];
}

void Matrix4L::set(int i, int j, double value) {
  m[j][i] = value;
}

Matrix4L Matrix4L::multiply(Matrix4L o) {
  Matrix4L tmp = Matrix4L();
  tmp.set(0, 0, matrixSubMult(this, &o, 0, 0));
  tmp.set(0, 1, matrixSubMult(this, &o, 0, 1));
  tmp.set(0, 2, matrixSubMult(this, &o, 0, 2));
  tmp.set(0, 3, matrixSubMult(this, &o, 0, 3));
  tmp.set(1, 0, matrixSubMult(this, &o, 1, 0));
  tmp.set(1, 1, matrixSubMult(this, &o, 1, 1));
  tmp.set(1, 2, matrixSubMult(this, &o, 1, 2));
  tmp.set(1, 3, matrixSubMult(this, &o, 1, 3));
  tmp.set(2, 0, matrixSubMult(this, &o, 2, 0));
  tmp.set(2, 1, matrixSubMult(this, &o, 2, 1));
  tmp.set(2, 2, matrixSubMult(this, &o, 2, 2));
  tmp.set(2, 3, matrixSubMult(this, &o, 2, 3));
  tmp.set(3, 0, matrixSubMult(this, &o, 3, 0));
  tmp.set(3, 1, matrixSubMult(this, &o, 3, 1));
  tmp.set(3, 2, matrixSubMult(this, &o, 3, 2));
  tmp.set(3, 3, matrixSubMult(this, &o, 3, 3));
  return tmp;
}

double matrixSubMult(const Matrix4L* m, const Matrix4L* o, int row, int col) {
  double accum = 0.0;
  for (int i = 0; i < 4; i++) {
    accum += (m->get(row, i) * o->get(i, col));
  }

  return accum;
}

Vector4L Matrix4L::multiply(Vector4L o) {
  return Vector4L(
      matrixVectorSubMult(this, 0, o),
      matrixVectorSubMult(this, 1, o),
      matrixVectorSubMult(this, 2, o),
      matrixVectorSubMult(this, 3, o)
      );
}

double matrixVectorSubMult(const Matrix4L* m, int row, Vector4L o) {
  double accum = 0.0;
  for (int i = 0; i < 4; i++) {
    accum += (m->get(row, i) * o[i]);
  }

  return accum;
}

void Matrix4L::scale(double x, double y, double z) {
  this->identity();
  this->set(0, 0, x);
  this->set(1, 1, y);
  this->set(2, 2, z);
  this->set(3, 3, 1);
}

void Matrix4L::translate(double x, double y, double z){
  this->identity();
  this->set(0, 3, x);
  this->set(1, 3, y);
  this->set(2, 3, z);
}

void Matrix4L::transpose() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if(i < j) {
        double tmp = this->get(i, j);
        this->set(i, j, this->get(j, i));
        this->set(j, i, tmp);
      }
    }
  }
}

string Matrix4L::toString() {
  stringstream ret;

  ret << "Matrix4L:" << endl;
  for (int i = 0; i < 4; i++) {
    ret << "[";
    for (int j = 0; j < 4; j++) {
      ret << this->get(i, j);
      ret << ", ";
    }
    ret << "]" << endl;
  }

  return ret.str();
}

string Matrix4L::toStringRaw() {
  stringstream ret;

  ret << "Matrix4L:" << endl;
  for (int i = 0; i < 4; i++) {
    ret << "[";
    for (int j = 0; j < 4; j++) {
      ret << m[i][j];
      ret << ", ";
    }
    ret << "]" << endl;
  }

  return ret.str();
}

void Matrix4L::copyRot(Matrix4L* other) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            this->set(i, j, other->get(i, j));
        }
    }
}

/** Rotates the matrix according to a fictitious trackball, placed in
    the middle of the given window.
    The trackball is approximated by a Gaussian curve.
    The trackball coordinate system is: x=right, y=up, z=to viewer<BR>
    The origin of the mouse coordinates zero (0,0) is considered to be top left.
  @param width, height  window size in pixels
  @param fromX, fromY   mouse starting position in pixels
  @param toX, toY       mouse end position in pixels
*/
Matrix4L* Matrix4L::trackballRotation(int width, int height, int fromX, int fromY, int toX, int toY)
{
  const float TRACKBALL_SIZE = 1.3f;              // virtual trackball size (empirical value)
  Matrix4L mInv;                                   // inverse of ObjectView matrix
  Vector3L v1, v2;                                 // mouse drag positions in normalized 3D space
  float smallSize;                                // smaller window size between width and height
  float halfWidth, halfHeight;                    // half window sizes
  float angle;                                    // rotational angle
  float d;                                        // distance

  // Compute mouse coordinates in window and normalized to -1..1
  // ((0,0)=window center, (-1,-1) = bottom left, (1,1) = top right)
  halfWidth   = (float)width  / 2.0f;
  halfHeight  = (float)height / 2.0f;
  smallSize   = (halfWidth < halfHeight) ? halfWidth : halfHeight;
  v1[0]       = ((float)fromX - halfWidth)  / smallSize;
  v1[1]       = ((float)(height-fromY) - halfHeight) / smallSize;
  v2[0]       = ((float)toX   - halfWidth)  / smallSize;
  v2[1]       = ((float)(height-toY)   - halfHeight) / smallSize;

  // Compute z-coordinates on Gaussian trackball:
  d       = sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);
  v1[2]   = expf(-TRACKBALL_SIZE * d * d);
  d       = sqrtf(v2[0] * v2[0] + v2[1] * v2[1]);
  v2[2]   = expf(-TRACKBALL_SIZE * d * d);

  // Compute rotational angle:
  angle = v1.angle(v2);                          // angle = angle between v1 and v2

  // Compute rotational axis:
  v2 = v2.crossProduct(v1);                                  // v2 = v2 x v1 (cross product)

  // Convert axis coordinates (v2) from WCS to OCS:
  mInv.identity();
  mInv.copyRot(this);                             // copy rotational part of mv to mInv
  mInv.transpose();                             // invert orthogonal matrix mInv
  Vector4L vTmp = mInv.multiply(Vector4L(v2[0], v2[1], v2[2]));
  v2[0] = vTmp[0];
  v2[1] = vTmp[1];
  v2[2] = vTmp[2];
  v2.normalize();                                 // normalize v2 before rotation

  // Perform acutal model view matrix modification:
  this->rotateAxis(v2, -angle);      // rotate model view matrix
  return this;
}
