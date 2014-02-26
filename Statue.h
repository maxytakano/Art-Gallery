#ifndef _STATUE_H_
#define _STATUE_H_

#include "Object.h"

class Statue: public Object {
    public:
        Statue(): Object() {}
        Statue(float r, float g, float b): Object(r, g, b) {}

    protected:
        static bool loaded;
        static vector<shape_t> data;

        char* getName() {
            return (char*)("campus.obj");
        }

        bool* getLoaded() {
            return &loaded;
        }

        vector<shape_t>* getData() {
            return &data;
        }
};

bool Statue::loaded = false;
vector<shape_t> Statue::data = vector<shape_t>();

#endif
