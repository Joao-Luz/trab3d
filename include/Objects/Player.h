#pragma once

#include "Objects/Object.h"
#include "VectorMath.h"

namespace objects {

class Player : public Object{
private:
    float m_height;
    float m_radius;
    float m_max_velocity;
public:
    Player(float x, float y, float z, float height);

    Player() {}
    ~Player() {}

    float height() { return m_height; }
    void set_height(float height) { m_height = height; }

    float radius() { return m_radius; }
    void set_radius(float radius) { m_radius = radius; }

    float max_velocity() { return m_max_velocity; }
    void set_max_velocity(float max_velocity) { m_max_velocity = max_velocity; }

    void display();
    v3f center();
};

}