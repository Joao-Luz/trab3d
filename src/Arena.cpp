#include "Arena.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Arena::Arena(float width, float height) {
    m_dimensions = {width, height, height/2};

    for (int i = 0; i < (int)m_dimensions.x; i+=8) {
        LightSource light(
            {(float)i, m_dimensions.y/2, -m_dimensions.z/2, 1.0},
            {0.1, 0.1, 0.1, 0.1},
            {0.8, 0.8, 0.8, 0.3},
            {0.8, 0.8, 0.8, 0.3}
        );
        m_lights.push_back(light);
    }
}

void Arena::add_plataform(float x, float y, float width, float height) {
    Box plataform(x, y, 0, width, height, m_dimensions.z);
    plataform.set_color(0, 0, 1);
    m_plataforms.push_back(plataform);
}

void Arena::display() {

    for (auto light : m_lights) {
        light.display();
    }

    Plane floor(
        {0, 0, m_dimensions.z},
        {m_dimensions.x, 0, m_dimensions.z},
        {m_dimensions.x, 0, 0},
        {0, 0, 0}
    );
    floor.set_color(1, 0.5, 0.4);
    floor.display();

    Plane ceiling(
        {0, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        {0, m_dimensions.y, m_dimensions.z}
    );
    floor.set_color(1, 0.5, 0.4);
    ceiling.display();

    for (auto plataform : m_plataforms) {
        plataform.display();
    }
}

