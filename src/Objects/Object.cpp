#include "Objects/Object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Objects/Axes.h"

namespace objects {

Object::Object(v3f position, v3f normal, v3f direction) :
m_position{position},
m_normal{normal},
m_direction{direction} {
    m_scale = {1, 1, 1};

    m_color = {1, 1, 1, 1};
    m_emission = {0, 0, 0, 0};
    m_specular = {1, 1, 1, 1};
    
    m_shininess = 64;
    m_should_display = true;
    m_should_set_material = true;
    m_show_axes = false;

    m_angle = {0, 0, 0};
}

Object::Object(v3f position) : Object(position, {0, 0, 0}, {0, 0, 0}) {}

Object::Object(float x, float y, float z) : Object((v3f){x, y, z}) {}

void Object::set_material() {
    if (!m_should_set_material) return;

    float emission[] = { m_emission.x, m_emission.y, m_emission.z, m_emission.w };
    float color[] = { m_color.x, m_color.y, m_color.z, m_color.w};
    float specular[] = { m_specular.x, m_specular.y, m_specular.z, m_specular.w };
    float shininess[] = { m_shininess };

    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void Object::display_axes() {
    if (!m_show_axes) return;

    Axes axes(m_position, m_scale);
    axes.display();
}

}