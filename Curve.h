#ifndef _CURVE_H_
#define _CURVE_H_

#include "Vector3L.hpp"
#include "Vector4L.hpp"
#include "Matrix4L.hpp"

class Curve {
    public:
        Curve(Vector3L* v0, Vector3L* v1, Vector3L* v2, Vector3L* v3) {
            p0 = v0;
            p1 = v1;
            p2 = v2;
            p3 = v3;
            g = Matrix4L();
        }
        void calculateG();
        Vector4L point(double);
        Vector3L pointNormal(double);

        Vector3L* p0;
        Vector3L* p1;
        Vector3L* p2;
        Vector3L* p3;

    protected:
        Matrix4L g;
};

#endif
