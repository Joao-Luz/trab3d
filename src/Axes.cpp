#include "Axes.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Axes::Axes(v3f position, v3f dimensions) : 
m_position{position},
m_dimensions{dimensions} {}

void Axes::display() {

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        float color_x[] = {0, 0, 1, 1};
        glPushMatrix();
            glTranslatef(m_dimensions.x*0.5, 0, 0);
            glColor3fv(color_x);
            glTranslatef(m_position.x, m_position.y, m_position.z);
            glScalef(m_dimensions.x, 0.1, 0.1);
            glutSolidCube(1.0);
        glPopMatrix();

        float color_y[] = {0, 1, 0, 1};
        glPushMatrix();
            glTranslatef(0, m_dimensions.y*0.5, 0);
            glColor3fv(color_y);
            glTranslatef(m_position.x, m_position.y, m_position.z);
            glScalef(0.1, m_dimensions.y, 0.1);
            glutSolidCube(1.0);
        glPopMatrix();

        float color_z[] = {1, 0, 0, 1};
        glPushMatrix();
            glTranslatef(0, 0, m_dimensions.z*0.5);
            glColor3fv(color_z);
            glTranslatef(m_position.x, m_position.y, m_position.z);
            glScalef(0.1, 0.1, m_dimensions.z);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopAttrib();
}