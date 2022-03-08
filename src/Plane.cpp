#include "Plane.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Axes.h"

Plane::Plane(v3f a, v3f b, v3f c, v3f d) : m_a{a}, m_b{b}, m_c{c}, m_d{d} {
    m_width = (b - a).norm();
    m_length = (c - b).norm();
    m_position = (a + b + c + d)/4;
    m_show_axes = true;
}
void Plane::display() {
    GLfloat emission[] = { 0.00, 0.00, 0.00, 1.0};
    GLfloat color[] = { m_color.x, m_color.y, m_color.z, 1.0};
    GLfloat specular[] = { 0.5, 0.5, 0.5, 1.0};
    GLfloat shininess[] = { 50 };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    v3f normal = (m_b - m_a).cross(m_c - m_b);
    glNormal3f(normal.x, normal.y, normal.z);
    glPushMatrix();
        glBegin(GL_QUADS);
            glVertex3f(m_a.x, m_a.y, m_a.z);
            glVertex3f(m_b.x, m_b.y, m_b.z);
            glVertex3f(m_c.x, m_c.y, m_c.z);
            glVertex3f(m_d.x, m_d.y, m_d.z);
        glEnd();
    glPopMatrix();

    if (m_show_axes) {
        Axes axes(m_position, v3f{1, 1, 1});
        axes.display();
    }
}