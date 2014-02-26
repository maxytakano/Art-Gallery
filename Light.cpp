#include "Light.hpp"
#ifdef _WIN32
	#include <GL/glut.h>
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif

Light::Light(unsigned int e, float x, float y, float z, float a) {
    //m = model;
    pos = new float[4];
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = a;
    spotAngle = 180.0;
    spotExponent = 0;
    qa = 0.0;
    for(int i = 0; i < 5; i++) {
        enable[i] = false;
    }
    this->light = e;
}

Light::~Light() {
    delete[] pos;
    if(enable[0]) delete[] ambient;
    if(enable[1]) delete[] diffuse;
    if(enable[2]) delete[] specular;
    if(enable[3]) delete[] spotDirection;
}

void Light::set(unsigned int p, float* v) {
    switch(p) {
        case GL_AMBIENT:
            enable[0] = true;
            ambient = v;
            break;
        case GL_DIFFUSE:
            enable[1] = true;
            diffuse = v;
            break;
        case GL_SPECULAR:
            enable[2] = true;
            specular = v;
            break;
        case GL_SPOT_DIRECTION:
            enable[3] = true;
            spotDirection = v;
            break;
    }
}

void Light::setAmbient(float r, float g, float b, float a) {
    ambient = new float[4];
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
    ambient[3] = a;
    enable[0] = true;
}

void Light::setDiffuse(float r, float g, float b, float a) {
    diffuse = new float[4];
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;
    enable[1] = true;
}

void Light::setSpecular(float r, float g, float b, float a) {
    specular = new float[4];
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
    enable[2] = true;
}

void Light::setSpotDirection(float x, float y, float z) {
    spotDirection = new float[3];
    spotDirection[0] = x;
    spotDirection[1] = y;
    spotDirection[2] = z;
    enable[3] = true;
}

void Light::setSpotCutoff(float angle) {
    this->spotAngle = angle;
}

void Light::setSpotExponent(int exponent) {
    this->spotExponent = exponent;
}

void Light::setQuadriaticAttenuation(float val) {
    this->qa = val;
    enable[4] = true;
}

void Light::draw(bool enabled) {
    if(enabled) {
        glEnable(light);

        glLightfv(light, GL_POSITION, pos);
        if(enable[0]) glLightfv(light, GL_AMBIENT, ambient);
        if(enable[1]) glLightfv(light, GL_DIFFUSE, diffuse);
        if(enable[2]) glLightfv(light, GL_SPECULAR, specular);
        if(enable[3]) glLightfv(light, GL_SPOT_DIRECTION, spotDirection);
        if(enable[3]) glLightf(light, GL_SPOT_CUTOFF, spotAngle);
        if(enable[3]) glLighti(light, GL_SPOT_EXPONENT, spotExponent);
        if(enable[4]) glLightf(light, GL_CONSTANT_ATTENUATION, 0);
        if(enable[4]) glLightf(light, GL_QUADRATIC_ATTENUATION, qa);
    } else {
        glDisable(light);
        float tmp = pos[3];
        pos[3] = 2.0;
        glLightfv(light, GL_POSITION, pos);
        pos[3] = tmp;
    }
}
