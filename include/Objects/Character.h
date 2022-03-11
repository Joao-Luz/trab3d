#pragma once

#include "Arena.h"
#include "./Box.h"
#include "./Object.h"
#include "VectorMath.h"

namespace objects {

class Character : public Object{
protected:
    float m_height;
    float m_radius;
    float m_max_velocity;
    float m_jump_velocity;
    float m_clock;

    bool m_grounded;
public:
    Character(float x, float y, float z, float height, float radius);

    Character() {}
    ~Character() {}

    float height() { return m_height; }
    void set_height(float height) { m_height = height; }

    float radius() { return m_radius; }
    void set_radius(float radius) { m_radius = radius; }

    float max_velocity() { return m_max_velocity; }
    void set_max_velocity(float max_velocity) { m_max_velocity = max_velocity; }

    float jump_velocity() { return m_jump_velocity; }
    void set_jump_velocity(float jump_velocity) { m_jump_velocity = jump_velocity; }

    float clock() { return m_clock; }
    void set_clock(float clock) { m_clock = clock; }
    void increase_clock(float dt) { m_clock += dt; }

    bool grounded() { return m_grounded; }
    void set_grounded(float grounded) { m_grounded = grounded; }

    void set_center(v3f position) { m_position = position - (center() - m_position); }
    void set_center(float x, float y, float z) { set_center({x, y, z}); }
    void set_center_x(float x) { set_center({x, center().y, center().z}); }
    void set_center_y(float y) { set_center({center().x, y, center().z}); }
    void set_center_z(float z) { set_center({center().x, center().y, z}); }

    virtual void plataform_collision(Box plataform, float dt) {}
    virtual void arena_collision(Arena arena, float dt);

    void display();
    v3f center();
};

}