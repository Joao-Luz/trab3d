#include "Objects/Enemy.h"

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

}
