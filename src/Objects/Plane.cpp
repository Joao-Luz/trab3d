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

    m_tex_coordinates[0] = {0, 0};
    m_tex_coordinates[1] = {1, 0};
    m_tex_coordinates[2] = {1, 1};
    m_tex_coordinates[3] = {0, 1};

    m_normal = (pb-pa).cross(pc-pb).normalize();
    m_direction = (pd-pa).normalize();
    m_subdivisions_x = m_subdivisions_y = subdivisions;
    m_scale = {(pb-pa).norm(), 0, (pc-pb).norm()};
}

Plane::Plane(v3f pa, v3f pb, v3f pc, v3f pd) : Plane(pa, pb, pc, pd, 1) {}

Plane::Plane(v3f points[4]) : Plane(points[0], points[1], points[2], points[3]) {}
Plane::Plane(v3f points[4], v2f tex_coordinates[4]) : Plane(points[0], points[1], points[2], points[3]) {
    m_tex_coordinates[0] = tex_coordinates[0];
    m_tex_coordinates[1] = tex_coordinates[1];
    m_tex_coordinates[3] = tex_coordinates[3];
    m_tex_coordinates[4] = tex_coordinates[4];
}

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

    int width = 1;
    int height = 1;
    if (m_texture) {
        glBindTexture (GL_TEXTURE_2D, m_texture->id());
        width = m_texture->width()/2;
        height = m_texture->height()/2;
    }
    else
        glDisable(GL_TEXTURE_2D);

    v3f a = m_points[0];
    v3f b = m_points[1];
    v3f c = m_points[2];
    v3f d = m_points[3];

    v2f ta = m_tex_coordinates[0];
    v2f tb = m_tex_coordinates[1];
    v2f tc = m_tex_coordinates[2];
    v2f td = m_tex_coordinates[3];

    for (int i = 0; i < m_subdivisions_x; i++) {
        for (int j = 0; j < m_subdivisions_y; j++) {

            v3f curr_a = (a + (b-a)*((float)(i)/m_subdivisions_x) + (d-a)*((float)(j)/m_subdivisions_y)) - m_position;
            v3f curr_b = (a + (b-a)*((float)(i+1)/m_subdivisions_x) + (d-a)*((float)(j)/m_subdivisions_y)) - m_position;
            v3f curr_c = (a + (b-a)*((float)(i+1)/m_subdivisions_x) + (d-a)*((float)(j+1)/m_subdivisions_y)) - m_position;
            v3f curr_d = (a + (b-a)*((float)(i)/m_subdivisions_x) + (d-a)*((float)(j+1)/m_subdivisions_y)) - m_position;

            v2f curr_ta = (ta + (tb-ta)*((float)(i)/m_subdivisions_x) + (td-ta)*((float)(j)/m_subdivisions_y));
            v2f curr_tb = (ta + (tb-ta)*((float)(i+1)/m_subdivisions_x) + (td-ta)*((float)(j)/m_subdivisions_y));
            v2f curr_tc = (ta + (tb-ta)*((float)(i+1)/m_subdivisions_x) + (td-ta)*((float)(j+1)/m_subdivisions_y));
            v2f curr_td = (ta + (tb-ta)*((float)(i)/m_subdivisions_x) + (td-ta)*((float)(j+1)/m_subdivisions_y));

            glPushMatrix();
                glBegin(GL_QUADS);
                    glTexCoord2f (m_scale.x*curr_ta.x/width, m_scale.z*curr_ta.y/height);
                    glVertex3f(curr_a.x, curr_a.y, curr_a.z);

                    glTexCoord2f (m_scale.x*curr_tb.x/width, m_scale.z*curr_tb.y/height);
                    glVertex3f(curr_b.x, curr_b.y, curr_b.z);

                    glTexCoord2f (m_scale.x*curr_tc.x/width, m_scale.z*curr_tc.y/height);
                    glVertex3f(curr_c.x, curr_c.y, curr_c.z);

                    glTexCoord2f (m_scale.x*curr_td.x/width, m_scale.z*curr_td.y/height);
                    glVertex3f(curr_d.x, curr_d.y, curr_d.z);
                glEnd();
            glPopMatrix();
        }
    }

    glPopMatrix();
    glPopAttrib();
}

}