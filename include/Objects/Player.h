#pragma once

#include "Objects/Object.h"
#include "VectorMath.h"

namespace objects {

class Player : public Object{
private:
    float m_height;
public:
    Player(float x, float y, float z, float height);

    Player() {}
    ~Player() {}

    float height() { return m_height; }
    void set_height(float height) { m_height = height; }

    void display();
    v3f center();
};

}