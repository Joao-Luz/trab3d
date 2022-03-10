#include "Objects/Character.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Objects/Box.h"

namespace objects {
    
Character::Character(float x, float y, float z, float height, float radius) : Object(x, y, z) {
    m_height = height;
    m_scale = {height/2, height, height/2};
    m_show_axes = true;
    m_radius = radius;
    m_grounded = false;
    m_max_velocity = height*6;
    m_jump_velocity = height*12;
    m_clock = 0.0f;
}

void Character::display() {

    glPushMatrix();
    v3f center = this->center();

    glTranslatef(center.x, m_position.y, center.z);

    glRotatef(m_angle.x,1,0,0);
    glRotatef(90-m_angle.y,0,1,0);

    Box body({-m_scale.x/2, 0, -m_scale.z/2}, {m_height/2, m_height, m_height/2});
    body.set_show_axes(m_show_axes);
    body.display();

    glPopMatrix();
}

v3f Character::center() {
    Box body(m_position, {m_height/2, m_height, m_height/2});
    return body.center();
}

}