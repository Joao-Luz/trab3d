#include "Objects/Character.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Game.h"
#include "Objects/Box.h"

namespace objects {
    
Character::Character(float x, float y, float z, float height, float radius, Game* game) : Object(x, y, z) {
    m_height = height;
    m_scale = {height/2, height, height/2};
    m_show_axes = true;
    m_radius = radius;
    m_grounded = false;
    m_max_velocity = height*6;
    m_jump_velocity = height*12;
    m_clock = 0.0f;
    m_direction = {1, 0, 0};
    m_aim = {1, 0, 0};
    m_alive = true;
    m_initial_position = m_position;
    m_game = game;
}

v3f Character::center() {
    Box body(m_position, {m_height/2, m_height, m_height/2});
    return body.center();
}

Shot Character::shoot() {
    Shot shot(this->gun_position(), m_aim);
    return shot;
}

}