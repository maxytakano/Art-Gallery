#ifndef _LIGHT_H_
#define _LIGHT_H_

class Light
{
  protected:
    unsigned int light;
    float* pos;
    float* ambient;
    float* diffuse;
    float* specular;
    float* spotDirection;
    float spotAngle, qa;
    int spotExponent;
    bool enable[5];

  public:
    Light(unsigned int, float, float, float, float);
    ~Light();
    void draw(bool);
    void set(unsigned int, float*);
    void setAmbient(float, float, float, float);
    void setDiffuse(float, float, float, float);
    void setSpecular(float, float, float, float);
    void setSpotDirection(float, float, float);
    void setSpotCutoff(float angle);
    void setSpotExponent(int exponent);
    void setQuadriaticAttenuation(float);
};

#endif
