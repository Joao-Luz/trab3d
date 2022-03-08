#pragma once

#include "VectorMath.h"

class LightSource {
private:
    v4f m_position, m_ambient, m_diffuse, m_specular;
    bool m_active;
public:
    LightSource(float x, float y, float z);
    LightSource(v3f position);
    LightSource(v4f position, v4f ambient, v4f diffuse, v4f specular);
    LightSource() {}
    ~LightSource() {}

    bool active() { return m_active; }
    void set_active(bool val) { m_active = val; }

    void display();
};
