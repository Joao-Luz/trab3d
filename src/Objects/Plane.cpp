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

    m_normal = (pb-pa).cross(pc-pb).normalize();
    m_direction = (pd-pa).normalize();
    m_subdivisions_x = m_subdivisions_y = subdivisions;
    m_scale = {(pb-pa).norm(), 0, (pc-pb).norm()};
}

Plane::Plane(v3f pa, v3f pb, v3f pc, v3f pd) : Plane(pa, pb, pc, pd, 1) {}

Plane::Plane(v3f points[4]) : Plane(points[0], points[1], points[2], points[3]) {}

v3f Plane::center() {
    return (m_points[0] + m_points[1] + m_points[2] + m_points[3])/4;
}

void Plane::display() {
    if (!m_should_display) return;

    this->display_axes();

    glPushAttrib(GL_ENABLE_BIT);
    this->set_material();

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glNormal3f(m_normal.x, m_normal.y, m_normal.z);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//X
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//Y

    if (m_texture != -1)
        glBindTexture (GL_TEXTURE_2D, m_texture);
    else
        glDisable(GL_TEXTURE_2D);

    v3f a = m_points[0];
    v3f b = m_points[1];
    v3f c = m_points[2];
    v3f d = m_points[3];

    for (int i = 0; i < m_subdivisions_x; i++) {
        for (int j = 0; j < m_subdivisions_y; j++) {

            v3f curr_a = (a + (b-a)*((float)(i)/m_subdivisions_x) + (d-a)*((float)(j)/m_subdivisions_y)) - m_position;
            v3f curr_b = (a + (b-a)*((float)(i+1)/m_subdivisions_x) + (d-a)*((float)(j)/m_subdivisions_y)) - m_position;
            v3f curr_c = (a + (b-a)*((float)(i+1)/m_subdivisions_x) + (d-a)*((float)(j+1)/m_subdivisions_y)) - m_position;
            v3f curr_d = (a + (b-a)*((float)(i)/m_subdivisions_x) + (d-a)*((float)(j+1)/m_subdivisions_y)) - m_position;

            glPushMatrix();
                glBegin(GL_QUADS);
                    glTexCoord2f (m_scale.x*(float)i/m_subdivisions_x/16, m_scale.z*(float)(j)/m_subdivisions_y/16);
                    glVertex3f(curr_a.x, curr_a.y, curr_a.z);

                    glTexCoord2f (m_scale.x*(float)(i+1)/m_subdivisions_x/16, m_scale.z*(float)(j)/m_subdivisions_y/16);
                    glVertex3f(curr_b.x, curr_b.y, curr_b.z);

                    glTexCoord2f (m_scale.x*(float)(i+1)/m_subdivisions_x/16, m_scale.z*(float)(j+1)/m_subdivisions_y/16);
                    glVertex3f(curr_c.x, curr_c.y, curr_c.z);

                    glTexCoord2f (m_scale.x*(float)(i)/m_subdivisions_x/16, m_scale.z*(float)(j+1)/m_subdivisions_y/16);
                    glVertex3f(curr_d.x, curr_d.y, curr_d.z);
                glEnd();
            glPopMatrix();
        }
    }

    glPopMatrix();
    glPopAttrib();
}

}