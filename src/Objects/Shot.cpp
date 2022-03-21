#include "Objects/Shot.h"

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Objects/Box.h"
#include "Objects/Character.h"

namespace objects {

Shot::Shot(v3f position, v3f direction) : Object(position) {
    m_scale = {0.5, 0.5, 0.5};
    m_direction = direction;
    m_max_velocity = 10;
    m_color = {1, 0, 0, 1};
    m_emission = {1, 0, 0, 1};
    m_velocity = direction*m_max_velocity;
}

void Shot::display() {
    glPushMatrix();

        glTranslatef(m_position.x, m_position.y, m_position.z);
        v3f rotation_vector = (v3f){1, 0, 0}.cross(m_direction.normalize());
        float angle = 180*acos(m_direction.dot({1, 0, 0}))/M_PI;
        glRotatef(angle, rotation_vector.x, rotation_vector.y, rotation_vector.z);

        Box box(m_scale/(-2), m_scale);
        box.set_color(1, 0, 0, 1);
        box.set_emission(1, 0, 0, 1);
        box.display();

    glPopMatrix();
}

bool Shot::arena_collision(Arena arena, float dt) {
    v3f velocity = this->direction()*this->max_velocity();
    v3f next_position = this->center() + velocity * dt;

    if (next_position.x > arena.width() ||
        next_position.x < 0 ||
        next_position.y > arena.height() || 
        next_position.y < 0 || 
        next_position.z > arena.length() ||
        next_position.z < 0) {
        
        return true;
    }

    return false;
}

bool Shot::character_collision(Character character, float dt) {
    v3f velocity = this->direction()*this->max_velocity();
    v3f next_position = this->center() + velocity * dt;
    v3f next_character_position = character.center() + character.velocity() * dt;

    float height = character.height();
    float radius = character.radius();
    float distance = ((v3f){next_character_position.x, 0, next_character_position.z} - (v3f){m_position.x, 0, m_position.z}).norm();

    if (distance < radius &&
        next_position.y > next_character_position.y - height/2 &&
        next_position.y < next_character_position.y + height/2 ) {
        
        return true;
    } else {
        return false;
    }
}

bool Shot::plataform_collision(Box plataform, float dt) {
    v3f velocity = this->direction()*this->max_velocity();
    v3f next_position = this->center() + velocity * dt;

    float width = plataform.scale().x;
    float height = plataform.scale().y;
    float length = plataform.scale().z;
    v3f center = plataform.center();

    if (next_position.x > center.x - width/2 &&
        next_position.x < center.x + width/2 &&
        next_position.y > center.y - height/2 && 
        next_position.y < center.y + height/2 && 
        next_position.z > center.z - length/2 &&
        next_position.z < center.z + length/2) {
        
        return true;
    } else {
        return false;
    }
}

}
