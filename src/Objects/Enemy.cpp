#include "Objects/Enemy.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Objects/Box.h"

namespace objects {

void Enemy::plataform_collision(Box plataform, float dt) {
    v3f next_position = center() + m_velocity*dt;

    v3f center = plataform.center();
    float width = plataform.scale().x;
    float height = plataform.scale().y;
    if ((next_position.x + m_radius) > (center.x - width/2) && 
        (next_position.x - m_radius) < (center.x + width/2) &&
        (next_position.y + m_height/2) > (center.y - height/2) &&
        (next_position.y - m_height/2) < (center.y + height/2)) {
        
        // front
        if ((this->center().x + m_radius) < (plataform.center().x - width/2)) {
            set_velocity_x(-m_velocity.x);
        } else 

        // back
        if ((this->center().x - m_radius) > (plataform.center().x + width/2)) {
            set_velocity_x(-m_velocity.x);
        } else

        // top
        if ((this->center().y + m_height/2) < (plataform.center().y - height/2)) {
            set_velocity_y(0);
        } else

        // bottom
        if ((this->center().y - m_height/2) > (plataform.center().y + height/2)) {
            set_velocity_y(0);
            set_grounded(true);
            if ((this->center().x + m_radius) >= (center.x + width/2) ||
                (this->center().x - m_radius) <= (center.x - width/2)) {
                set_velocity_x(-m_velocity.x);
            }
        }
    }
}

void Enemy::display() {

    glPushMatrix();
    v3f center = this->center();

    glTranslatef(center.x, m_position.y, center.z);

    v3f rotation_vector = (v3f){1, 0, 0}.cross(m_direction);
    float angle = 180*acos(m_direction.normalize().dot({1, 0, 0}))/M_PI;

    glRotatef(angle, rotation_vector.x, rotation_vector.y, rotation_vector.z);

    Box body({-m_scale.x/2, 0, -m_scale.z/2}, {m_height/2, m_height, m_height/2});
    body.set_color(0.9, 0.2, 0.1, 1);
    body.set_show_axes(m_show_axes);
    body.display();

    glPopMatrix();
}

}
