#pragma once

#include "Objects/Object.h"
#include "VectorMath.h"

namespace objects {

class Axes : public Object {
private:
public:
    Axes(float x, float y, float z);
    Axes(float x, float y, float z, float w, float h, float l);
    Axes(v3f position, v3f scale);
    Axes() {}
    ~Axes() {}

    void display();
};

}