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
    std::vector<LightSource> m_lights;
    bool m_active_lights;
    
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

    bool active_lights() { return m_active_lights; }
    void set_active_lights(bool val) { m_active_lights = val; }

    void load_arena_from_file(const char* path);
    void add_plataform(float x, float y, float width, float height);
    void display();
};
