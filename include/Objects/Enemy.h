#pragma once

#include "Animation.h"
#include "Arena.h"
#include "./Box.h"
#include "./Character.h"
#include "./Shot.h"

namespace objects {

class Enemy : public Character{
private:
    Box m_gun;
    Box m_head, m_body;
    Box m_upper_right_arm, m_lower_right_arm;
    Box m_upper_left_arm, m_lower_left_arm;
    Box m_upper_right_leg, m_lower_right_leg;
    Box m_upper_left_leg, m_lower_left_leg;

    std::unordered_map<std::string, Animation*> m_animations;
    Animation* m_current_animation;
public:
    Enemy(float x, float y, float z, float height, float radius, Game* game);

    Enemy() {}
    ~Enemy() {}

    void set_current_animation(std::string name);
    void update_current_animation(float dt);

    void plataform_collision(Box plataform, float dt);
    void arena_collision(Arena arena, float dt);
    Shot shoot(v3f player_position);
    v3f gun_position();
    void display();
};


}