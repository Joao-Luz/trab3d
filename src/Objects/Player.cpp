#include "Objects/Player.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Objects/Box.h"

namespace objects {

void Player::plataform_collision(Box plataform, float dt) {
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
            this->set_velocity_x(0);
        } else 

        // back
        if ((this->center().x - m_radius) > (plataform.center().x + width/2)) {
            this->set_velocity_x(0);
        } else

        // top
        if ((this->center().y + m_height/2) < (plataform.center().y - height/2)) {
            this->set_velocity_y(0);
        } else

        // bottom
        if ((this->center().y - m_height/2) > (plataform.center().y + height/2)) {
            this->set_velocity_y(0);
            this->set_grounded(true);
        }
    }
}

void Player::enemy_collision(Enemy* enemy, float dt) {
    v3f next_player_position = this->center() + m_velocity*dt;
    v3f next_enemy_position = enemy->center() + enemy->velocity()*dt;

    if ((next_player_position.y + m_height/2) > (next_enemy_position.y - enemy->height()/2) &&
        (next_player_position.y - m_height/2) < (next_enemy_position.y + enemy->height()/2)) {
        
        next_player_position.y = 0;
        next_enemy_position.y = 0;
        float distance = (next_enemy_position - next_player_position).norm();

        if (distance < (m_radius + enemy->radius())) {
            // bottom top
            if ((this->center().y - m_height/2) > (enemy->center().y + enemy->height()/2)) {
                this->set_velocity_y(0);
                this->set_grounded(true);
            // sides
            } else {
                float a = m_velocity.x * enemy->velocity().x;
                a = a > 0 ? 1 : -1;
                float b = m_velocity.z * enemy->velocity().z;
                b = b > 0 ? 1 : -1;
                enemy->set_velocity_x(a*enemy->velocity().x);
                enemy->set_velocity_z(b*enemy->velocity().z);

                this->set_velocity_x(0);
                this->set_velocity_z(0);
            }
        }
    }
}

void Player::display() {

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
            arm.display();
        glPopMatrix();

        Box body({-m_scale.x/2, 0, -m_scale.z/2}, {m_height/2, m_height, m_height/2});
        body.set_show_axes(m_show_axes);
        body.display();

    glPopMatrix();
}

void Player::arena_collision(Arena arena, float dt) {
    v3f next_position = this->center() + m_velocity*dt;

    if ((next_position.z - m_radius) < 0) {
        set_velocity_z(0);
        set_center_z(m_radius);
    }

    if ((next_position.z + m_radius) > arena.length()) {
        set_velocity_z(0);
        set_center_z(arena.length() - m_radius);
    }

    if ((next_position.x - m_radius) < 0) {
        set_velocity_x(0);
        set_center_x(m_radius);
    }

    if ((next_position.x + m_radius) > arena.width()) {
        set_velocity_x(0);
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

}
