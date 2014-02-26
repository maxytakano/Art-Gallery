#include <math.h>
#include <iostream>

#include "Matrix4.h"

Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
} 

Matrix4::Matrix4(
  double m00, double m01, double m02, double m03,
  double m10, double m11, double m12, double m13,
  double m20, double m21, double m22, double m23,
  double m30, double m31, double m32, double m33 )
{
  m[0][0] = m00;
  m[0][1] = m01;
  m[0][2] = m02;
  m[0][3] = m03;
  m[1][0] = m10;
  m[1][1] = m11;
  m[1][2] = m12;
  m[1][3] = m13;
  m[2][0] = m20;
  m[2][1] = m21;
  m[2][2] = m22;
  m[2][3] = m23;
  m[3][0] = m30;
  m[3][1] = m31;
  m[3][2] = m32;
  m[3][3] = m33;
}

void Matrix4::set(
  double m00, double m01, double m02, double m03,
  double m10, double m11, double m12, double m13,
  double m20, double m21, double m22, double m23,
  double m30, double m31, double m32, double m33 )
{
  m[0][0] = m00;
  m[0][1] = m01;
  m[0][2] = m02;
  m[0][3] = m03;
  m[1][0] = m10;
  m[1][1] = m11;
  m[1][2] = m12;
  m[1][3] = m13;
  m[2][0] = m20;
  m[2][1] = m21;
  m[2][2] = m22;
  m[2][3] = m23;
  m[3][0] = m30;
  m[3][1] = m31;
  m[3][2] = m32;
  m[3][3] = m33;

}



double* Matrix4::getPointer()
{
  return &m[0][0];
}

void Matrix4::identity()
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

int Matrix4::get(int x,int y) {
    return m[x][y];
}

Matrix4 Matrix4::multiply(const Matrix4& m2) {     // why const and &????
    Matrix4 newM;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < 4; inner++) {
                newM.m[row][col] += m[row][inner] * m2.m[inner][col];
            }
        }
    }

    return newM;
}

// Corrected for ROW MAJOR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Vector4 Matrix4::multiply(const Vector4& a) {
    Vector4 v2;

    for (int row = 0; row < 4; row++) {
        //for (int row = 0; row < 4; row++) {
            // Multiply the row of A by the rowumn of B to get the row, rowumn of product.
            for (int inner = 0; inner < 4; inner++) {
                v2.v[row] += m[row][inner] * a.v[inner];
            }
        //}
    }

    return v2;
}

void Matrix4::rotateX(double angle) {
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

// angle in radians
void Matrix4::rotateY(double angle)
{
    m[0][0] = cos(angle);
    m[0][1] = 0;
    m[0][2] = sin(angle);
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = -sin(angle);
    m[2][1] = 0;
    m[2][2] = cos(angle);
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

Matrix4 Matrix4::rotateZ(double angle) {
    Matrix4 m2;
    m2.m[0][0] = cos(angle);
    m2.m[0][1] = -sin(angle);
    m2.m[0][2] = 0;
    m2.m[0][3] = 0;
    m2.m[1][0] = sin(angle);
    m2.m[1][1] = cos(angle);
    m2.m[1][2] = 0;
    m2.m[1][3] = 0;
    m2.m[2][0] = 0;
    m2.m[2][1] = 0;
    m2.m[2][2] = 1;
    m2.m[2][3] = 0;
    m2.m[3][0] = 0;
    m2.m[3][1] = 0;
    m2.m[3][2] = 0;
    m2.m[3][3] = 1;
    return m2;
}

Matrix4 Matrix4::rotateArbitrary(double radians, double x, double y, double z) { // ??
    Matrix4 result = Matrix4();
    double ax = x;
    double ay = y;
    double az = z;

    result.m[0][0] = 1 + (1 - cos(radians)) * (ax * ax - 1);
    result.m[0][1] = -az * sin(radians) + (1 - cos(radians)) * ax * ay;
    result.m[0][2] = ay * sin(radians) + (1 - cos(radians)) * ax * az;
    result.m[0][3] = 0;
    result.m[1][0] = az * sin(radians) + (1 - cos(radians)) * ay * ax;
    result.m[1][1] = 1 + (1 - cos(radians)) * (ay * ay - 1);
    result.m[1][2] = -ax * sin(radians) + (1 - cos(radians)) * ay *  az;
    result.m[1][3] = 0;
    result.m[2][0] = -ay * sin(radians) + (1 - cos(radians)) * az * ax;
    result.m[2][1] = ax * sin(radians) + (1 - cos(radians)) * az * ay;
    result.m[2][2] = 1 + (1 - cos(radians)) * (az * az - 1);
    result.m[2][3] = 0;
    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;
    return result;
}

Matrix4 Matrix4::scaling(double s, double t, double r) {  // ??
    Matrix4 m2;

    m2.m[0][0] = s;
    m2.m[0][1] = 0;
    m2.m[0][2] = 0;
    m2.m[0][3] = 0;
    m2.m[1][0] = 0;
    m2.m[1][1] = t;
    m2.m[1][2] = 0;
    m2.m[1][3] = 0;
    m2.m[2][0] = 0;
    m2.m[2][1] = 0;
    m2.m[2][2] = r;
    m2.m[2][3] = 0;
    m2.m[3][0] = 0;
    m2.m[3][1] = 0;
    m2.m[3][2] = 0;
    m2.m[3][3] = 1;

    return m2;
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
Matrix4 Matrix4::trackballRotation(int width, int height, int fromX, int fromY, int toX, int toY)
{
  const float TRACKBALL_SIZE = 1.3f;              // virtual trackball size (empirical value)
  Matrix4 mInv;                                   // inverse of ObjectView matrix
  Vector3 v1, v2;                                 // mouse drag positions in normalized 3D space
  Vector4 v3;
  float smallSize;                                // smaller window size between width and height
  float halfWidth, halfHeight;                    // half window sizes
  float angle;                                    // rotational angle
  float d;                                        // distance

  // Compute mouse coordinates in window and normalized to -1..1
  // ((0,0)=window center, (-1,-1) = bottom left, (1,1) = top right)
  halfWidth   = (float)width  / 2.0f;
  halfHeight  = (float)height / 2.0f;
  smallSize   = (halfWidth < halfHeight) ? halfWidth : halfHeight;
  v1.v[0]       = ((float)fromX - halfWidth)  / smallSize;
  v1.v[1]       = ((float)(height-fromY) - halfHeight) / smallSize;
  v2.v[0]       = ((float)toX   - halfWidth)  / smallSize;
  v2.v[1]       = ((float)(height-toY)   - halfHeight) / smallSize;

  // Compute z-coordinates on Gaussian trackball:
  d       = sqrtf(v1.v[0] * v1.v[0] + v1.v[1] * v1.v[1]);
  v1.v[2]   = expf(-TRACKBALL_SIZE * d * d);
  d       = sqrtf(v2.v[0] * v2.v[0] + v2.v[1] * v2.v[1]);
  v2.v[2]   = expf(-TRACKBALL_SIZE * d * d);

  // Compute rotational angle:
  angle = acos(v1.dot(v2) / (v1.magnitude() * v2.magnitude()) );                          // angle = angle between v1 and v2

  // Compute rotational axis:
  v2 = v2.cross(v2, v1);                                  // v2 = v2 x v1 (cross product)

  // Convert axis coordinates (v2) from WCS to OCS:
  mInv.identity();
  mInv.copyRot(*this);                             // copy rotational part of mv to mInv
  mInv.transpose();                             // invert orthogonal matrix mInv

	v3.v[0] = v2.v[0];
	v3.v[1] = v2.v[1];
	v3.v[2] = v2.v[2];
	v3.v[3] = 0;

	v3 = mInv.multiply(v3);                             // v2 = v2 x mInv (matrix multiplication)

	v2.v[0] = v3.v[0];
	v2.v[1] = v3.v[1];
	v2.v[2] = v3.v[2];
	
    v2.normalize();                                 // normalize v2 before rotation

	//Matrix4 temp = rotateArbitrary(-angle, v2[0], v2[1], v2[2]); 
	//temp.printMatrix();

  // Perform acutal model view matrix modification:
  return rotateArbitrary(-angle, v2[0], v2[1], v2[2]);      // rotate model view matrix
}

void Matrix4::copyRot(Matrix4 r) {
	Matrix4::m[0][0] = r.m[0][0];
	Matrix4::m[0][1] = r.m[0][1];
	Matrix4::m[0][2] = r.m[0][2];
	Matrix4::m[0][3] = 0;

	Matrix4::m[1][0] = r.m[1][0];
	Matrix4::m[1][1] = r.m[1][1];
	Matrix4::m[1][2] = r.m[1][2];
	Matrix4::m[1][3] = 0;

	Matrix4::m[2][0] = r.m[2][0];
	Matrix4::m[2][1] = r.m[2][1];
	Matrix4::m[2][2] = r.m[2][2];
	Matrix4::m[2][3] = 0;

	Matrix4::m[3][0] = 0;
	Matrix4::m[3][1] = 0;
	Matrix4::m[3][2] = 0;
	Matrix4::m[3][3] = 1;

	
}

Matrix4 Matrix4::translation(double t1, double t2, double t3) { // ??
    Matrix4 m2;
    
    m2.m[0][0]= 1;
    m2.m[0][1] = 0;
    m2.m[0][2] = 0;
    m2.m[0][3] = t1;
    m2.m[1][0] = 0;
    m2.m[1][1] = 1;
    m2.m[1][2] = 0;
    m2.m[1][3] = t2;
    m2.m[2][0] = 0;
    m2.m[2][1] = 0;
    m2.m[2][2] = 1;
    m2.m[2][3] = t3;
    m2.m[3][0] = 0;
    m2.m[3][1] = 0;
    m2.m[3][2] = 0;
    m2.m[3][3] = 1;

    return m2;
}

void Matrix4::printMatrix() { 
    for(int i = 0; i < 4; i++) { 
        for(int j = 0; j < 4; j++) {
            std::cout << m[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

void Matrix4::printoned() {
    for (int i = 0; i < 16; i++ ) {
        std::cout << oned[i] << ' ';
    }
}

Matrix4 Matrix4::transpose() { 
    Matrix4 result = Matrix4();
    result.m[0][0] = m[0][0];
    result.m[0][1] = m[1][0];
    result.m[0][2] = m[2][0];
    result.m[0][3] = m[3][0];
    result.m[1][0] = m[0][1];
    result.m[1][1] = m[1][1];
    result.m[1][2] = m[2][1];
    result.m[1][3] = m[3][1];
    result.m[2][0] = m[0][2];
    result.m[2][1] = m[1][2];
    result.m[2][2] = m[2][2];
    result.m[2][3] = m[3][2];
    result.m[3][0] = m[0][3];
    result.m[3][1] = m[1][3];
    result.m[3][2] = m[2][3];
    result.m[3][3] = m[3][3];
    return result;
}

double* Matrix4::getValues() {
  //return oned;
    return invOut;

}

void Matrix4::invert() {
    int i;
    double det;

    inv[0] = oned[5]  * oned[10] * oned[15] - 
             oned[5]  * oned[11] * oned[14] - 
             oned[9]  * oned[6]  * oned[15] + 
             oned[9]  * oned[7]  * oned[14] +
             oned[13] * oned[6]  * oned[11] - 
             oned[13] * oned[7]  * oned[10];

    inv[4] = -oned[4]  * oned[10] * oned[15] + 
              oned[4]  * oned[11] * oned[14] + 
              oned[8]  * oned[6]  * oned[15] - 
              oned[8]  * oned[7]  * oned[14] - 
              oned[12] * oned[6]  * oned[11] + 
              oned[12] * oned[7]  * oned[10];

    inv[8] = oned[4]  * oned[9] * oned[15] - 
             oned[4]  * oned[11] * oned[13] - 
             oned[8]  * oned[5] * oned[15] + 
             oned[8]  * oned[7] * oned[13] + 
             oned[12] * oned[5] * oned[11] - 
             oned[12] * oned[7] * oned[9];

    inv[12] = -oned[4]  * oned[9] * oned[14] + 
               oned[4]  * oned[10] * oned[13] +
               oned[8]  * oned[5] * oned[14] - 
               oned[8]  * oned[6] * oned[13] - 
               oned[12] * oned[5] * oned[10] + 
               oned[12] * oned[6] * oned[9];

    inv[1] = -oned[1]  * oned[10] * oned[15] + 
              oned[1]  * oned[11] * oned[14] + 
              oned[9]  * oned[2] * oned[15] - 
              oned[9]  * oned[3] * oned[14] - 
              oned[13] * oned[2] * oned[11] + 
              oned[13] * oned[3] * oned[10];

    inv[5] = oned[0]  * oned[10] * oned[15] - 
             oned[0]  * oned[11] * oned[14] - 
             oned[8]  * oned[2] * oned[15] + 
             oned[8]  * oned[3] * oned[14] + 
             oned[12] * oned[2] * oned[11] - 
             oned[12] * oned[3] * oned[10];

    inv[9] = -oned[0]  * oned[9] * oned[15] + 
              oned[0]  * oned[11] * oned[13] + 
              oned[8]  * oned[1] * oned[15] - 
              oned[8]  * oned[3] * oned[13] - 
              oned[12] * oned[1] * oned[11] + 
              oned[12] * oned[3] * oned[9];

    inv[13] = oned[0]  * oned[9] * oned[14] - 
              oned[0]  * oned[10] * oned[13] - 
              oned[8]  * oned[1] * oned[14] + 
              oned[8]  * oned[2] * oned[13] + 
              oned[12] * oned[1] * oned[10] - 
              oned[12] * oned[2] * oned[9];

    inv[2] = oned[1]  * oned[6] * oned[15] - 
             oned[1]  * oned[7] * oned[14] - 
             oned[5]  * oned[2] * oned[15] + 
             oned[5]  * oned[3] * oned[14] + 
             oned[13] * oned[2] * oned[7] - 
             oned[13] * oned[3] * oned[6];

    inv[6] = -oned[0]  * oned[6] * oned[15] + 
              oned[0]  * oned[7] * oned[14] + 
              oned[4]  * oned[2] * oned[15] - 
              oned[4]  * oned[3] * oned[14] - 
              oned[12] * oned[2] * oned[7] + 
              oned[12] * oned[3] * oned[6];

    inv[10] = oned[0]  * oned[5] * oned[15] - 
              oned[0]  * oned[7] * oned[13] - 
              oned[4]  * oned[1] * oned[15] + 
              oned[4]  * oned[3] * oned[13] + 
              oned[12] * oned[1] * oned[7] - 
              oned[12] * oned[3] * oned[5];

    inv[14] = -oned[0]  * oned[5] * oned[14] + 
               oned[0]  * oned[6] * oned[13] + 
               oned[4]  * oned[1] * oned[14] - 
               oned[4]  * oned[2] * oned[13] - 
               oned[12] * oned[1] * oned[6] + 
               oned[12] * oned[2] * oned[5];

    inv[3] = -oned[1] * oned[6] * oned[11] + 
              oned[1] * oned[7] * oned[10] + 
              oned[5] * oned[2] * oned[11] - 
              oned[5] * oned[3] * oned[10] - 
              oned[9] * oned[2] * oned[7] + 
              oned[9] * oned[3] * oned[6];

    inv[7] = oned[0] * oned[6] * oned[11] - 
             oned[0] * oned[7] * oned[10] - 
             oned[4] * oned[2] * oned[11] + 
             oned[4] * oned[3] * oned[10] + 
             oned[8] * oned[2] * oned[7] - 
             oned[8] * oned[3] * oned[6];

    inv[11] = -oned[0] * oned[5] * oned[11] + 
               oned[0] * oned[7] * oned[9] + 
               oned[4] * oned[1] * oned[11] - 
               oned[4] * oned[3] * oned[9] - 
               oned[8] * oned[1] * oned[7] + 
               oned[8] * oned[3] * oned[5];

    inv[15] = oned[0] * oned[5] * oned[10] - 
              oned[0] * oned[6] * oned[9] - 
              oned[4] * oned[1] * oned[10] + 
              oned[4] * oned[2] * oned[9] + 
              oned[8] * oned[1] * oned[6] - 
              oned[8] * oned[2] * oned[5];

    det = oned[0] * inv[0] + oned[1] * inv[4] + oned[2] * inv[8] + oned[3] * inv[12];

    det = 1.0 / det;

    for (i = 0; i < 16; i++) {
        invOut[i] = inv[i] * det;
    }
}
