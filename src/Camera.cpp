#include "Camera.h"

Camera::Camera(float x, float y, float z) {
    m_position = {x, y, z};
    m_direction = {-1, 0, 0};
    m_up = {0, 1, 0};
    m_angle = {0, 0};
    m_mode = orbital;
}