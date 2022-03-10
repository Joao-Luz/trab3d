#include "Objects/LightSource.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Objects/Box.h"

namespace objects {

LightSource::LightSource(float x, float y, float z, int id) :
LightSource({x, y, z, 0}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, id) {}

LightSource::LightSource(v3f position, int id) :
LightSource(position.x, position.y, position.z, id) {}

LightSource::LightSource(v4f position, v4f ambient, v4f diffuse, v4f specular, int id) : 
Object(position),
m_ambient_light{ambient},
m_diffuse_light{diffuse},
m_specular_light{specular},
m_light_id{id} {
    m_active = true;
}

void LightSource::display() {
    if (!m_active) {
        glDisable(GL_LIGHT0+m_light_id);
        return;
    }

    float light_params[4][4] = {
        {m_ambient_light.x,  m_ambient_light.y,  m_ambient_light.z,  m_ambient_light.w},
        {m_diffuse_light.x,  m_diffuse_light.y,  m_diffuse_light.z,  m_diffuse_light.w},
        {m_specular_light.x, m_specular_light.y, m_specular_light.z, m_specular_light.w},
        {m_position.x, m_position.y, m_position.z, 1.0f}
    };

    glEnable(GL_LIGHT0+m_light_id);
    glLightf(GL_LIGHT0+m_light_id, GL_LINEAR_ATTENUATION, 0.1);
    glLightfv(GL_LIGHT0+m_light_id, GL_AMBIENT,  &light_params[0][0]);
    glLightfv(GL_LIGHT0+m_light_id, GL_DIFFUSE,  &light_params[1][0]);
    glLightfv(GL_LIGHT0+m_light_id, GL_SPECULAR, &light_params[2][0]);
    glLightfv(GL_LIGHT0+m_light_id, GL_POSITION, &light_params[3][0]);
}

}