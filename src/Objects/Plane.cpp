#include "Objects/Plane.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

namespace objects {

Plane::Plane(v3f pa, v3f pb, v3f pc, v3f pd, int subdivisions) : Object(pa) {
    m_points[0] = pa;
    m_points[1] = pb;
    m_points[2] = pc;
    m_points[3] = pd;

    m_normal = (pb - pa).cross(pc - pb);
    m_direction = {1, 0, 0};
    m_subdivisions = subdivisions;
}

Plane::Plane(v3f pa, v3f pb, v3f pc, v3f pd) : Plane(pa, pb, pc, pd, 1) {}

Plane::Plane(v3f points[4]) : Plane(points[0], points[1], points[2], points[3]) {}

v3f Plane::center() {
    return (m_points[0] + m_points[1] + m_points[2] + m_points[3])/4;
}

void Plane::display() {
    if (!m_should_display) return;

    this->display_axes();

    this->set_material();

    glNormal3f(m_normal.x, m_normal.y, m_normal.z);

    v3f a = m_points[0];
    v3f b = m_points[1];
    v3f c = m_points[2];
    v3f d = m_points[3];

    for (int i = 0; i < m_subdivisions; i++) {
        for (int j = 0; j < m_subdivisions; j++) {

            v3f curr_a = (a + (b-a)*((float)(i)/m_subdivisions) + (d-a)*((float)(j)/m_subdivisions));
            v3f curr_b = (a + (b-a)*((float)(i+1)/m_subdivisions) + (d-a)*((float)(j)/m_subdivisions));
            v3f curr_c = (a + (b-a)*((float)(i+1)/m_subdivisions) + (d-a)*((float)(j+1)/m_subdivisions));
            v3f curr_d = (a + (b-a)*((float)(i)/m_subdivisions) + (d-a)*((float)(j+1)/m_subdivisions));

            glPushMatrix();
                glBegin(GL_QUADS);
                    glVertex3f(curr_a.x, curr_a.y, curr_a.z);
                    glVertex3f(curr_b.x, curr_b.y, curr_b.z);
                    glVertex3f(curr_c.x, curr_c.y, curr_c.z);
                    glVertex3f(curr_d.x, curr_d.y, curr_d.z);
                glEnd();
            glPopMatrix();
        }
    }
}

}