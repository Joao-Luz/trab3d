#pragma once

#include <unordered_map>

#include "Animation.h"
#include "./Box.h"
#include "./Character.h"
#include "./Enemy.h"
#include "./LightSource.h"

namespace objects {

class Player : public Character{
private:
    LightSource m_flashlight;
    bool m_flashlight_on;
    bool m_won;

    Box m_gun;
    Box m_head, m_body;
    Box m_upper_right_arm, m_lower_right_arm;
    Box m_upper_left_arm, m_lower_left_arm;
    Box m_upper_right_leg, m_lower_right_leg;
    Box m_upper_left_leg, m_lower_left_leg;

    std::unordered_map<std::string, Animation*> m_animations;
    Animation* m_current_animation;
public:
    Player(float x, float y, float z, float height, float radius);

    Player() {}
    ~Player() {}

    bool flashlight_on() { return m_flashlight_on; }
    void set_flashlight_on(bool val) { m_flashlight_on = val; }

    bool won() { return m_won; }
    void set_won(bool val) { m_won = val; }

    void set_current_animation(std::string name);
    void update_current_animation(float dt);

    void plataform_collision(Box plataform, float dt);
    void arena_collision(Arena arena, float dt);
    void enemy_collision(Enemy* enemy, float dt);
    v3f gun_position();
    void display();
};

}