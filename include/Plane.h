#pragma once

#include "VectorMath.h"

class Plane {
private:
    v3f m_position, m_color, m_a, m_b, m_c, m_d;
    float m_width, m_length;
    bool m_show_axes;
public:

    Plane(v3f a, v3f b, v3f c, v3f d);
    Plane() {}
    ~Plane() {}

    v3f position() { return m_position; }
    void set_position(v3f position) { m_position = position; }
    void set_position(float x, float y, float z) { m_position = {x, y, z}; }
    void translate(float dx, float dy, float dz) { m_position = m_position + (v3f){dx, dy, dz}; }
    void translate(v3f displacement) { m_position = m_position + displacement; }

    void set_dimensions(float w, float l) { m_width = w; m_length = l; }
    float width() { return m_width; }
    float length() { return m_length; }

    v3f color() { return m_color; }
    void set_color(v3f color) { m_color = color; }
    void set_color(float r, float g, float b) { m_color = {r, g, b}; }

    bool show_axes() { return m_show_axes; }
    void set_show_axes(bool val) { m_show_axes = val; }

    void display();
};
