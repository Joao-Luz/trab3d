#include "Objects/Text.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

namespace objects {

Text::Text(v3f position, std::string text) : Object(position), m_text{text} {
}
Text::Text(float x, float y, float z, std::string text) : Text({x, y, z}, text) {}

void Text::display() {
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho (0, m_screen_width, 0, m_screen_height, -1, 1);

        glPushAttrib(GL_ENABLE_BIT);
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glColor3f(m_color.x, m_color.y, m_color.z);
            glRasterPos3f(m_position.x-m_text.size()*4.5, m_position.y, m_position.z);

            for (auto character : m_text) {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, character);
            }

        glPopAttrib();
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

}
