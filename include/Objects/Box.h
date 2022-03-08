#pragma once

#include "./Object.h"
#include "../VectorMath.h"

namespace objects {

class Box : public Object {
private:
    int m_faces_subdivisions;
public:
    Box(float x, float y, float z);
    Box(v3f position);
    Box(v3f position, v3f scale);
    Box(v3f position, v3f scale, int faces_subdivisions);

    Box() {}
    ~Box() {}

    int faces_subdivisions() { return m_faces_subdivisions; }
    void set_faces_subdivisions(int faces_subdivisions) { m_faces_subdivisions = faces_subdivisions; }

    v3f center();

    void display();
};

}