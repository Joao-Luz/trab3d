#pragma once

#include "Arena.h"
#include "VectorMath.h"
#include "./Box.h"
#include "./Object.h"

namespace objects {
    
class Character;

class Shot : public Object{
private:
    float m_max_velocity;
public:
    Shot(v3f position, v3f direction);

    Shot() {}
    ~Shot() {}

    float max_velocity() { return m_max_velocity; }
    void set_max_velocity(float max_velocity) { m_max_velocity = max_velocity; }
    bool arena_collision(Arena arena, float dt);
    bool character_collision(Character character, float dt);
    bool plataform_collision(Box plataform, float dt);

    void display();
};
}