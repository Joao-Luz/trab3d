#pragma once

#include "Objects/Object.h"
#include "VectorMath.h"

namespace objects {

class Camera : public Object {

private:
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

    camera_mode mode() { return m_mode; }
    void set_mode(camera_mode mode) { m_mode = mode; }
};

}