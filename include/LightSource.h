#pragma once

#include "VectorMath.h"

class LightSource {
private:
    v4f m_position, m_ambient, m_diffuse, m_specular;
    bool m_active;
    int m_light_id;
public:
    LightSource(float x, float y, float z, int id);
    LightSource(v3f position, int id);
    LightSource(v4f position, v4f ambient, v4f diffuse, v4f specular, int id);
    LightSource() {}
    ~LightSource() {}

    bool active() { return m_active; }
    void set_active(bool val) { m_active = val; }

    void display();
};
