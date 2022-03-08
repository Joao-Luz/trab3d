#pragma once

#include "VectorMath.h"

class Player {
private:
    v3f m_position;
    float m_height;
    bool m_show_axes;
public:
    Player(float x, float y, float z, float height);

    Player() {}
    ~Player() {}

    v3f position() { return m_position; }
    void set_position(v3f position) { m_position = position; }
    void set_position(float x, float y, float z) { m_position = {x, y, z}; }
    void translate(float dx, float dy, float dz) { m_position = m_position + (v3f){dx, dy, dz}; }
    void translate(v3f displacement) { m_position = m_position + displacement; }

    float height() { return m_height; }
    void set_height(float height) { m_height = height; }

    void display();
    bool show_axes() { return m_show_axes; }
    void set_show_axes(bool val) { m_show_axes = val; }
    v3f center();
};
