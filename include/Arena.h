#pragma once

#include <vector>

#include "Objects/Box.h"
#include "Objects/LightSource.h"
#include "Objects/Plane.h"
#include "VectorMath.h"
#include "Texture.h"

class Game;

class Arena {
private:
    v3f m_dimensions;
    std::vector<objects::Box> m_plataforms;
    objects::LightSource m_lights[8];
    bool m_lights_on;
    bool m_show_axes;

    Game* m_game;

public:
    Arena(float width, float height, Game* game);
    Arena() {}
    ~Arena() {}

    v3f dimensions() { return m_dimensions; }
    void set_dimensions(v3f dimensions) { m_dimensions = dimensions; }
    void set_dimensions(float w, float h, float l) { m_dimensions = {w, h, l}; }
    float width() { return m_dimensions.x; }
    float height() { return m_dimensions.y; }
    float length() { return m_dimensions.z; }

    bool lights_on() { return m_lights_on; }
    void set_lights_on(bool val) { m_lights_on = val; }

    std::vector<objects::Box> plataforms() { return m_plataforms; }

    void load_arena_from_file(const char* path);
    void add_plataform(float x, float y, float width, float height);
    void display();

    bool show_axes() { return m_show_axes; }
    void set_show_axes(bool show_axes) { m_show_axes = show_axes; }
};
