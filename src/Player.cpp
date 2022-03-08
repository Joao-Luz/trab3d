#include "Player.h"

#include "Box.h"

Player::Player(float x, float y, float z, float height) {
    m_position = (v3f){x, y, z};
    m_height = height;
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