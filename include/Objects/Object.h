#pragma once

#include "../VectorMath.h"
#include <string>

namespace objects {

class Object {
protected:
    v3f m_position;
    v3f m_normal;
    v3f m_direction;
    v3f m_scale;
    v3f m_velocity;
    
    v4f m_color;
    v4f m_emission;
    v4f m_specular;
    float m_shininess;

    bool m_should_display;
    bool m_show_axes;
    bool m_should_set_material;

    int m_texture;

    void set_material();

    v3f m_angle;
public:
    Object(float x, float y, float z);
    Object(v3f position);
    Object(v3f position, v3f normal, v3f direction);

    v3f position() { return m_position; }
    virtual v3f center() { return m_position; }
    void set_position(v3f position) { m_position = position; }
    void set_position(float x, float y, float z) { m_position = {x, y, z}; }
    void translate(float dx, float dy, float dz) { m_position = m_position + (v3f){dx, dy, dz}; }
    void translate(v3f displacement) { m_position = m_position + displacement; }

    v3f normal() { return m_normal; }
    void set_normal(v3f normal) { m_normal = normal; }
    void set_normal(float x, float y, float z) { m_normal = {x, y, z}; }

    v3f direction() { return m_direction; }
    void set_direction(v3f direction) { m_direction = direction; }
    void set_direction(float x, float y, float z) { m_direction = {x, y, z}; }

    v3f scale() { return m_scale; }
    void set_scale(v3f scale) { m_scale = scale; }
    void set_scale(float x, float y, float z) { m_scale = {x, y, z}; }

    v3f velocity() { return m_velocity; }
    void set_velocity(v3f velocity) { m_velocity = velocity; }
    void set_velocity(float x, float y, float z) { m_velocity = {x, y, z}; }
    void increase_velocity(v3f dv) { m_velocity = m_velocity + dv; }
    void increase_velocity(float x, float y, float z) { m_velocity = m_velocity + (v3f){x, y, z}; }

    v4f color() { return m_color; }
    void set_color(v4f color) { m_color = color; }
    void set_color(float r, float g, float b, float a) { m_color = {r, g, b, a}; }

    v4f emission() { return m_emission; }
    void set_emission(v4f emission) { m_emission = emission; }
    void set_emission(float r, float g, float b, float a) { m_emission = {r, g, b, a}; }

    v4f specular() { return m_specular; }
    void set_specular(v4f specular) { m_specular = specular; }
    void set_specular(float r, float g, float b, float a) { m_specular = {r, g, b, a}; }

    float shininess() { return m_shininess; }
    void set_shininess(float shininess) { m_shininess = shininess; }

    bool should_display() { return m_should_display; }
    void set_should_display(bool should_display) { m_should_display = should_display; }

    bool show_axes() { return m_show_axes; }
    void set_show_axes(bool show_axes) { m_show_axes = show_axes; }

    bool should_set_material() { return m_should_set_material; }
    void set_should_set_material(bool should_set_material) { m_should_set_material = should_set_material; }

    void set_angle(float yz, float xz, float xy) { m_angle = {yz, xz, xy}; }

    float angle_yz() { return m_angle.x; }
    void set_angle_yz(float angle) { m_angle.x = angle; }

    float angle_xz() { return m_angle.y; }
    void set_angle_xz(float angle) { m_angle.y = angle; }

    float angle_xy() { return m_angle.z; }
    void set_angle_xy(float angle) { m_angle.z = angle; }

    void set_texture(std::string name);
    void set_texture(int texture) { m_texture = texture; }

    void display_axes();

    virtual void display() {}

    Object() {}
    ~Object() {}
};

}