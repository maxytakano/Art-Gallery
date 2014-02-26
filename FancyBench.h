#ifndef _FANCYBENCH_H_
#define _FANCYBENCH_H_

#include "Object.h"

class FancyBench: public Object {
    public:
        FancyBench(): Object() {}
        FancyBench(float r, float g, float b): Object(r, g, b) {}

    protected:
        static bool loaded;
        static vector<shape_t> data;

        char* getName() {
            return (char*)("bench_v01.obj");
        }

        bool* getLoaded() {
            return &loaded;
        }

        vector<shape_t>* getData() {
            return &data;
        }
};

bool FancyBench::loaded = false;
vector<shape_t> FancyBench::data = vector<shape_t>();

#endif
