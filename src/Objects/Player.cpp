#include "Objects/Player.h"

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

}