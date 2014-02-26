#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Matrix4.hpp"

class Viewport
{
  protected:
    int px0, px1, py0, py1;
    Matrix4 m;

    void update();
    
  public:
    Viewport(int, int, int, int);
    int x0() const;
    int x1() const;
    int y0() const;
    int y1() const;

    void x0(int);
    void x1(int);
    void y0(int);
    void y1(int);

    Matrix4 &getMatrix();
};

#endif
