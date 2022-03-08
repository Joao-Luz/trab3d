#include "Arena.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Objects/Plane.h"

Arena::Arena(float width, float height) {
    m_dimensions = {width, height, height/2};

    for (int i = 0; i < 8; i++) {
        float x = i*m_dimensions.x/8;
        objects::LightSource light(
            {x, m_dimensions.y/2, 0, 1.0},
            {0.0, 0.0, 0.0, 0.1}, // ambient
            {0.8, 0.8, 0.8, 0.3}, // difuse
            {0.8, 0.8, 0.8, 0.3}, // specular
            i
        );
        m_lights[i] = light;
    }
}

void Arena::add_plataform(float x, float y, float width, float height) {
    objects::Box plataform({x, y, 0}, {width, height, m_dimensions.z}, 4);
    plataform.set_color(0, 0, 1, 1);
    m_plataforms.push_back(plataform);
}

void Arena::display() {

    for (int i = 0; i < 8; i++) {
        m_lights[i].set_active(m_active_lights[i]);
        m_lights[i].set_show_axes(m_show_axes);
        m_lights[i].display();
    }

    objects::Plane floor(
        {0, 0, m_dimensions.z},
        {m_dimensions.x, 0, m_dimensions.z},
        {m_dimensions.x, 0, 0},
        {0, 0, 0},
        20
    );
    floor.set_color(1, 0.5, 0.4, 1);
    floor.set_show_axes(m_show_axes);
    floor.display();

    objects::Plane ceiling(
        {0, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        {0, m_dimensions.y, m_dimensions.z},
        20
    );
    ceiling.set_color(1, 0.5, 0.4, 1);
    ceiling.set_show_axes(m_show_axes);
    ceiling.display();

    for (auto plataform : m_plataforms) {
        plataform.set_show_axes(m_show_axes);
        plataform.display();
    }
}

