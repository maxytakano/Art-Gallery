#ifndef _SPOT_H_
#define _SPOT_H_

#include "Object.h"

class Spot: public Object {
    public:
        Spot(): Object() {}
        Spot(float r, float g, float b): Object(r, g, b) {}

    protected:
        char* getName() {
            return (char*)("spotlight.obj");
        }
};

#endif
