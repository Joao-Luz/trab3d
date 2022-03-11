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
    m_direction = {1, 0, 0};
}

Camera::Camera(float x, float y, float z) : Camera((v3f){x, y, z}) { }

void Camera::init() {
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    gluPerspective(m_fov, m_aspect, m_near, m_far);
}

void Camera::look_at(Object object) {
    look_at(object.center());
}

void Camera::look_at(v3f point) {
    gluLookAt(
        m_position.x,
        m_position.y,
        m_position.z,

        point.x, 
        point.y, 
        point.z, 

        m_normal.x,
        m_normal.y,
        m_normal.z
    );
}

}