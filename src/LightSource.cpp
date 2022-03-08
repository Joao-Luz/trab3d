#include "LightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Box.h"

LightSource::LightSource(float x, float y, float z) :
LightSource({x, y, z, 0}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}) {}

LightSource::LightSource(v3f position) :
LightSource (position.x, position.y, position.z) {}

LightSource::LightSource(v4f position, v4f ambient, v4f diffuse, v4f specular) : 
m_position{position},
m_ambient{ambient},
m_diffuse{diffuse},
m_specular{specular} {
    m_active = true;
}

void LightSource::display() {
    if (!m_active) return;

    Box box(m_position, {0.3, 0.3, 0.3});
    box.set_color(1, 1, 0);
    box.set_emission(1, 1, 0);
    box.display();

    float light_params[4][4] = {
        {m_ambient.x,  m_ambient.y,  m_ambient.z,  m_ambient.w},
        {m_diffuse.x,  m_diffuse.y,  m_diffuse.z,  m_diffuse.w},
        {m_specular.x, m_specular.y, m_specular.z, m_specular.w},
        {m_position.x, m_position.y, m_position.z, m_position.w}
    };
    glLightfv(GL_LIGHT0, GL_AMBIENT,  &light_params[0][0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  &light_params[1][0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, &light_params[2][0]);
    glLightfv(GL_LIGHT0, GL_POSITION, &light_params[3][0]);
}