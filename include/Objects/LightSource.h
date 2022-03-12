#pragma once

#include "VectorMath.h"
#include "Objects/Object.h"

namespace objects {

class LightSource : public Object {
private:
    v4f m_ambient_light, m_diffuse_light, m_specular_light;
    int m_light_id;
    bool m_active;

    bool m_spotlight;
    float m_spot_cutoff;
    v3f m_spot_direction;
public:
    LightSource(float x, float y, float z, int id);
    LightSource(v3f position, int id);
    LightSource(v4f position, v4f ambient, v4f diffuse, v4f specular, int id);

    LightSource() {}
    ~LightSource() {}

    bool active() { return m_active; }
    void set_active(bool val) { m_active = val; }

    bool spotlight() { return m_spotlight; }
    void set_spotlight(bool val) { m_spotlight = val; }

    float spot_cutoff() { return m_spot_cutoff; }
    void set_spot_cutoff(float val) { m_spot_cutoff = val; }

    v3f spot_direction() { return m_spot_direction; }
    void set_spot_direction(v3f spot_direction) { m_spot_direction = spot_direction; }
    void set_spot_direction(float x, float y, float z) { m_spot_direction = {x, y, z}; }

    void display();
};

}