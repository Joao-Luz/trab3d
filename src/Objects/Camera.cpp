#include "Objects/Camera.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

namespace objects {

Camera::Camera(v3f position, float fov, float aspect, float near, float far) :
Object(position),
m_fov{fov},
m_aspect{aspect},
m_near{near},
m_far{far} {
    m_angle = {0, 0, 0};
    m_mode = first_person;
}

Camera::Camera(float x, float y, float z) : Camera((v3f){x, y, z}) { }

void Camera::init() {
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    gluPerspective(m_fov, m_aspect, m_near, m_far);
}

}