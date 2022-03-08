#pragma once

#include "VectorMath.h"

class Camera {

private:
    v3f m_position, m_direction, m_up;

    struct {
        float yz, xz;
    } m_angle;
public:
    enum camera_mode{
        orbital = 0,
        first_person = 1,
        aiming = 2
    };
    camera_mode m_mode;

    Camera(float x, float y, float z);
    Camera() {}
    ~Camera() {}

    v3f direction() { return m_direction; }
    void set_direction(v3f direction) { m_direction = direction; }
    void set_direction(float x, float y, float z) { m_direction = {x, y, z}; }

    v3f up() { return m_up; }
    void set_up(v3f up) { m_up = up; }
    void set_up(float x, float y, float z) { m_up = {x, y, z}; }

    v3f position() { return m_position; }
    void set_position(v3f position) { m_position = position; }
    void set_position(float x, float y, float z) { m_position = {x, y, z}; }
    void translate(float dx, float dy, float dz) { m_position = m_position + (v3f){dx, dy, dz}; }
    void translate(v3f displacement) { m_position = m_position + displacement; }

    camera_mode mode() { return m_mode; }
    void set_mode(camera_mode mode) { m_mode = mode; }

    void set_angle(float yz, float xz) { m_angle = {yz, xz}; }

    float angle_yz() { return m_angle.yz; }
    void set_angle_yz(float angle) { m_angle.yz = angle; }

    float angle_xz() { return m_angle.xz; }
    void set_angle_xz(float angle) { m_angle.xz = angle; }
};
