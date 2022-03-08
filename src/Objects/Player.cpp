#include "Objects/Player.h"

#include "Objects/Box.h"

namespace objects {
    
Player::Player(float x, float y, float z, float height) : Object(x, y, z) {
    m_height = height;
    m_scale = {height/2, height, height/2};
    m_show_axes = true;
}

void Player::display() {
    Box body(m_position, {m_height/2, m_height, m_height/2});
    body.set_show_axes(m_show_axes);
    body.display();
}

v3f Player::center() {
    Box body(m_position, {m_height/2, m_height, m_height/2});
    return body.center();
}

}