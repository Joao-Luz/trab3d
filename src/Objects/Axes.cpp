#include "Objects/Axes.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#include "Objects/Box.h"

namespace objects {

Axes::Axes(v3f position, v3f scale) : Object(position) {
    m_scale = scale;
}

void Axes::display() {

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        glColor3f(0, 0, 1);
        Box x_axis((v3f){ m_position.x-0.05f, m_position.y-0.05f, m_position.z-0.05f }, { m_scale.x, 0.1, 0.1 });
        x_axis.set_should_set_material(false);
        x_axis.set_show_axes(false);
        x_axis.display();

        glColor3f(0, 1, 0);
        Box y_axis((v3f){ m_position.x-0.05f, m_position.y-0.05f, m_position.z-0.05f }, { 0.1, m_scale.y, 0.1 });
        y_axis.set_should_set_material(false);
        y_axis.set_show_axes(false);
        y_axis.display();

        glColor3f(1, 0, 0);
        Box z_axis((v3f){ m_position.x-0.05f, m_position.y-0.05f, m_position.z-0.05f }, { 0.1, 0.1, m_scale.z });
        z_axis.set_should_set_material(false);
        z_axis.set_show_axes(false);
        z_axis.display();

    glPopAttrib();
}

}