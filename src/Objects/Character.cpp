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
    m_direction = {1, 0, 0};
}

v3f Character::center() {
    Box body(m_position, {m_height/2, m_height, m_height/2});
    return body.center();
}

void Character::arena_collision(Arena arena, float dt) {
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

}