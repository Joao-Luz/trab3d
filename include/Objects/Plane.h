#pragma once

#include "./Object.h"
#include "../VectorMath.h"

namespace objects {

class Plane : public Object {
private:
    v3f m_points[4];
    int m_subdivisions;
public:
    Plane(v3f pa, v3f pb, v3f pc, v3f pd);
    Plane(v3f pa, v3f pb, v3f pc, v3f pd, int subdivisions);
    Plane(v3f points[4]);

    Plane() {}
    ~Plane() {}

    int subdivisions() { return m_subdivisions; }
    void set_subdivisions(int subdivisions) { m_subdivisions = subdivisions; }

    v3f center();

    void display();
};

}
