#pragma once

#include "./Object.h"
#include "../VectorMath.h"

namespace objects {

class Plane : public Object {
private:
    v3f m_points[4];
    v2f m_tex_coordinates[4];
    int m_subdivisions_x, m_subdivisions_y;
public:
    Plane(v3f pa, v3f pb, v3f pc, v3f pd);
    Plane(v3f pa, v3f pb, v3f pc, v3f pd, int subdivisions);
    Plane(v3f pa, v3f pb, v3f pc, v3f pd, v2f ta, v2f tb, v2f tc, v2f td, int subdivisions);
    Plane(v3f points[4]);
    Plane(v3f points[4], v2f tex_coordinates[4]);

    Plane() {}
    ~Plane() {}

    int subdivisions_x() { return m_subdivisions_x; }
    void set_subdivisions_x(int subdivisions_x) { m_subdivisions_x = subdivisions_x; }

    int subdivisions_y() { return m_subdivisions_y; }
    void set_subdivisions_y(int subdivisions_y) { m_subdivisions_y = subdivisions_y; }

    v3f center();

    void display();
};

}
