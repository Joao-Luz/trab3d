#pragma once

#include "./Character.h"
#include "./Enemy.h"
#include "./LightSource.h"

namespace objects {

class Player : public Character{
private:
    LightSource m_flashlight;
    bool m_flashlight_on;
public:
    Player(float x, float y, float z, float height, float radius);

    Player() {}
    ~Player() {}

    bool flashlight_on() { return m_flashlight_on; }
    void set_flashlight_on(bool val) { m_flashlight_on = val; }

    void plataform_collision(Box plataform, float dt);
    void arena_collision(Arena arena, float dt);
    void enemy_collision(Enemy* enemy, float dt);
    v3f gun_position();
    void display();
};

}