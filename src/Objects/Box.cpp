#include "Objects/Box.h"

#include "Objects/Plane.h"

#include <iostream>

namespace objects {

Box::Box(v3f position, v3f scale, int faces_subdivisions) : Object(position, {0, 1, 0}, {1, 0, 0}) {
    m_scale = scale;
    m_faces_subdivisions = faces_subdivisions;
}

Box::Box(v3f position, v3f scale) : Box(position, scale, 1) { }

Box::Box(v3f position) : Box(position, {1, 1, 1}) { }

Box::Box(float x, float y, float z) : Box((v3f){x, y, z}) { }

v3f Box::center() {
    return m_position + m_scale/2;
}

void Box::display() {
    if (!m_should_display) return;

    if (m_show_axes) {
        this->display_axes();

    }

    this->set_material();

    // bottom
    Plane bottom(
        {m_position},
        {m_position + (v3f){m_scale.x, 0, 0}},
        {m_position + (v3f){m_scale.x, 0, m_scale.z}},
        {m_position + (v3f){0, 0, m_scale.z}},
        m_faces_subdivisions
    );
    bottom.set_should_set_material(false);
    bottom.display();


    // top
    Plane top(
        {m_position + (v3f){0, m_scale.y, 0}},
        {m_position + (v3f){0, m_scale.y, m_scale.z}},
        {m_position + (v3f){m_scale.x, m_scale.y, m_scale.z}},
        {m_position + (v3f){m_scale.x, m_scale.y, 0}},
        m_faces_subdivisions
    );
    top.set_should_set_material(false);
    top.display();

    // back
    Plane back(
        {m_position},
        {m_position + (v3f){0, 0, m_scale.z}},
        {m_position + (v3f){0, m_scale.y, m_scale.z}},
        {m_position + (v3f){0, m_scale.y, 0}},
        m_faces_subdivisions
    );
    back.set_should_set_material(false);
    back.display();

    // front
    Plane front(
        {m_position + (v3f){m_scale.x, 0, 0}},
        {m_position + (v3f){m_scale.x, m_scale.y, 0}},
        {m_position + (v3f){m_scale.x, m_scale.y, m_scale.z}},
        {m_position + (v3f){m_scale.x, 0, m_scale.z}},
        m_faces_subdivisions
    );
    front.set_should_set_material(false);
    front.display();

    // left
    Plane left(
        {m_position},
        {m_position + (v3f){0, m_scale.y, 0}},
        {m_position + (v3f){m_scale.x, m_scale.y, 0}},
        {m_position + (v3f){m_scale.x, 0, 0}},
        m_faces_subdivisions
    );
    left.set_should_set_material(false);
    left.display();

    // right
    Plane right(
        {m_position + (v3f){0, 0, m_scale.z}},
        {m_position + (v3f){m_scale.x, 0, m_scale.z}},
        {m_position + (v3f){m_scale.x, m_scale.y, m_scale.z}},
        {m_position + (v3f){0, m_scale.y, m_scale.z}},
        m_faces_subdivisions
    );
    right.set_should_set_material(false);
    right.display();
}

}
