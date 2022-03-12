#pragma once

#include "Arena.h"
#include "./Box.h"
#include "./Character.h"
#include "./Shot.h"

namespace objects {

class Enemy : public Character{
private:
public:
    Enemy(float x, float y, float z, float height, float radius) : Character(x, y, z, height, radius) {}

    Enemy() {}
    ~Enemy() {}

    void plataform_collision(Box plataform, float dt);
    void arena_collision(Arena arena, float dt);
    Shot shoot(v3f player_position);
    v3f gun_position();
    void display();
};


}