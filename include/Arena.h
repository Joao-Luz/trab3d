#pragma once

#include <vector>

#include "Box.h"
#include "LightSource.h"
#include "Plane.h"
#include "VectorMath.h"

class Arena {
private:
    v3f m_dimensions;
    std::vector<Box> m_plataforms;
    LightSource m_lights[8];
    bool m_active_lights[8];
    
public:
    Arena(float width, float height);
    Arena() {}
    ~Arena() {}

    v3f dimensions() { return m_dimensions; }
    void set_dimensions(v3f dimensions) { m_dimensions = dimensions; }
    void set_dimensions(float w, float h, float l) { m_dimensions = {w, h, l}; }
    float width() { return m_dimensions.x; }
    float height() { return m_dimensions.y; }
    float length() { return m_dimensions.z; }

    bool active_lights(int i) { return m_active_lights[i]; }
    void set_active_lights(bool vals[8]) { for (int i = 0; i < 8; i++) m_active_lights[i] = vals[i]; }
    void set_active_lights(bool val, int i) { m_active_lights[i] = val; }

    void load_arena_from_file(const char* path);
    void add_plataform(float x, float y, float width, float height);
    void display();
};
