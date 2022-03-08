#pragma once

#include "VectorMath.h"
#include "Objects/Object.h"

namespace objects {

class LightSource : public Object {
private:
    v4f m_ambient_light, m_diffuse_light, m_specular_light;
    int m_light_id;
    bool m_active;
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

}