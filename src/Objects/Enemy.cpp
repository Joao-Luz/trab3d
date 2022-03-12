#include "Objects/Enemy.h"

#include <iostream>
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

        glPushMatrix();
            glTranslatef(0, 3*m_height/4, m_height/4);
            float cos = m_direction.dot(m_aim);
            if (cos > 1) cos = 1.0f;
            if (cos < -1) cos = -1.0f;
            angle = 180*acos(cos)/M_PI;

            if (m_aim.y <= 0) angle = -angle;

            glRotatef(angle, 0, 0, 1);
            Box arm({-m_height/12, 0, 0}, {m_height/2, m_height/6, m_height/6});
            arm.set_color(0.9, 0.2, 0.1, 1);
            arm.display();
        glPopMatrix();


        Box body({-m_scale.x/2, 0, -m_scale.z/2}, {m_height/2, m_height, m_height/2});
        body.set_color(0.9, 0.2, 0.1, 1);
        body.set_show_axes(m_show_axes);
        body.display();

    glPopMatrix();
}

void Enemy::arena_collision(Arena arena, float dt) {
    v3f next_position = this->center() + m_velocity*dt;

    if ((next_position.z - m_radius) < 0) {
        set_velocity_z(-m_velocity.z);
        set_center_z(m_radius);
    }

    if ((next_position.z + m_radius) > arena.length()) {
        set_velocity_z(-m_velocity.z);
        set_center_z(arena.length() - m_radius);
    }

    if ((next_position.x - m_radius) < 0) {
        set_velocity_x(-m_velocity.x);
        set_center_x(m_radius);
    }

    if ((next_position.x + m_radius) > arena.width()) {
        set_velocity_x(-m_velocity.x);
        set_center_x(arena.width() - m_radius);
    }

    if ((next_position.y - m_height/2) < 0) {
        set_velocity_y(0);
        set_center_y(m_height/2);
        set_grounded(true);
    }

    if ((next_position.y + m_height/2) > arena.height()) {
        set_velocity_y(0);
        set_center_y(arena.height() - m_height/2);
    }

}

v3f Enemy::gun_position() {
    v3f right = m_direction.cross({0, 1, 0}).normalize();

    return this->center() + v3f(0, m_height/3, 0) + (right*m_height/3) + (m_aim*m_height/2);
}

Shot Enemy::shoot(v3f player_position) {
    v3f direction = (player_position - this->gun_position()).normalize();
    Shot shot(this->gun_position(), direction);
    return shot;
}

}
