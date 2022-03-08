#pragma once

#include "VectorMath.h"

class Box {
private:
    v3f m_position;
    v3f m_color;
    v3f m_dimensions;
    bool m_show_axes;

    v4f m_emission = { 0.00, 0.00, 0.00, 1.0};
    v4f m_specular = { 1.0, 1.0, 1.0, 1.0};
    float m_shininess = 1;

public:
    Box(float x, float y, float z);
    Box(v3f position);
    Box(float x, float y, float z, float w, float h, float l);
    Box(v3f position, v3f dimensions);
    Box() {}
    ~Box() {}

    v3f position() { return m_position; }
    v3f center() { return m_position + m_dimensions/2; }
    void set_position(v3f position) { m_position = position; }
    void set_position(float x, float y, float z) { m_position = {x, y, z}; }
    void translate(float dx, float dy, float dz) { m_position = m_position + (v3f){dx, dy, dz}; }
    void translate(v3f displacement) { m_position = m_position + displacement; }

    v3f color() { return m_color; }
    void set_color(v3f color) { m_color = color; }
    void set_color(float r, float g, float b) { m_color = {r, g, b}; }

    v3f emission() { return m_emission; }
    void set_emission(v4f emission) { m_emission = emission; }
    void set_emission(float r, float g, float b) { m_emission = {r, g, b, 1}; }

    v3f specular() { return m_specular; }
    void set_specular(v4f specular) { m_specular = specular; }
    void set_specular(float r, float g, float b) { m_specular = {r, g, b, 1}; }

    float shininess() { return m_shininess; }
    void set_shininess(float shininess) { m_shininess = shininess; }

    v3f dimensions() { return m_dimensions; }
    void set_dimensions(v3f dimensions) { m_dimensions = dimensions; }
    void set_dimensions(float w, float h, float l) { m_dimensions = {w, h, l}; }
    float width() { return m_dimensions.x; }
    float height() { return m_dimensions.y; }
    float length() { return m_dimensions.z; }

    bool show_axes() { return m_show_axes; }
    void set_show_axes(bool val) { m_show_axes = val; }

    void display();
};
