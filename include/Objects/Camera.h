#pragma once

#include "Objects/Object.h"
#include "VectorMath.h"

namespace objects {

class Camera : public Object {

private:
    float m_fov, m_aspect, m_near, m_far;
public:
    enum camera_mode{
        orbital = 0,
        first_person = 1,
        aiming = 2
    };
    camera_mode m_mode;

    Camera(float x, float y, float z);
    Camera(v3f position, float fov = 90, float aspect = 1, float near = 1, float far = 500);
    Camera() {}
    ~Camera() {}

    camera_mode mode() { return m_mode; }
    void set_mode(camera_mode mode) { m_mode = mode; }
    void look_at(objects::Object object);
    void look_at(v3f point);

    void init();
};

}