#include "Box.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Axes.h"

Box::Box(v3f position, v3f dimensions) : 
m_position{position},
m_dimensions{dimensions} {
    m_color = {1, 1, 1};
    m_show_axes = true;

    m_emission = { 0, 0, 0, 1};
    m_specular = { 1, 1, 1, 1};
    m_shininess = 1;
}

Box::Box(float x, float y, float z) : Box(x, y, z, 1, 1, 1) {}

Box::Box(float x, float y, float z, float w, float h, float l) : Box({x, y, z}, {w, h, l}) {}

void Box::display() {
    GLfloat emission[] = { m_emission.x, m_emission.y, m_emission.z, m_emission.w };
    GLfloat color[] = { m_color.x, m_color.y, m_color.z, 1.0};
    GLfloat specular[] = { m_specular.x, m_specular.y, m_specular.z, m_specular.w };
    GLfloat shininess[] = { m_shininess };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glPushMatrix();
        glTranslatef(m_position.x+m_dimensions.x/2, m_position.y+m_dimensions.y/2, m_position.z+m_dimensions.z/2);
        glScalef(m_dimensions.x, m_dimensions.y, m_dimensions.z);
        glutSolidCube(1.0);
    glPopMatrix();

    if (m_show_axes) {
        Axes axes(m_position, m_dimensions);
        axes.display();
    }
}