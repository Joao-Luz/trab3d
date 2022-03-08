#pragma once

#include "VectorMath.h"

class Axes {
private:
    v3f m_position;
    v3f m_dimensions;
public:
    Axes(float x, float y, float z);
    Axes(v3f position);
    Axes(float x, float y, float z, float w, float h, float l);
    Axes(v3f position, v3f dimensions);
    Axes() {}
    ~Axes() {}

    v3f position() { return m_position; }
    void set_position(v3f position) { m_position = position; }
    void set_position(float x, float y, float z) { m_position = {x, y, z}; }
    void translate(float dx, float dy, float dz) { m_position = m_position + (v3f){dx, dy, dz}; }
    void translate(v3f displacement) { m_position = m_position + displacement; }

    v3f dimensions() { return m_dimensions; }
    void set_dimensions(v3f dimensions) { m_dimensions = dimensions; }
    void set_dimensions(float w, float h, float l) { m_dimensions = {w, h, l}; }
    float width() { return m_dimensions.x; }
    float height() { return m_dimensions.y; }
    float length() { return m_dimensions.z; }

    void display();
};
