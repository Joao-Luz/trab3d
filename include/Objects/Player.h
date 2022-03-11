#pragma once

#include "./Character.h"
#include "./Enemy.h"

namespace objects {

class Player : public Character{
private:
public:
    Player(float x, float y, float z, float height, float radius) : Character(x, y, z, height, radius) {}

    Player() {}
    ~Player() {}

    void plataform_collision(Box plataform, float dt);
    void arena_collision(Arena arena, float dt);
    void enemy_collision(Enemy* enemy, float dt);
    void display();
};

}