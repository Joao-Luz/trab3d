#include "Objects/Camera.h"

namespace objects {

Camera::Camera(float x, float y, float z) : Object({x, y, z}, {0, 1, 0}, {1, 0, 0}) {
    m_angle = {0, 0};
    m_mode = orbital;
}

}