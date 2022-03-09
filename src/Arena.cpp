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
            {x, m_dimensions.y/2, 0.5, 1.0},
            {0.05, 0.05, 0.05, 0.03}, // ambient
            {0.05, 0.05, 0.05, 0.03}, // difuse
            {0.05, 0.05, 0.05, 0.03}, // specular
            i
        );
        m_lights[i] = light;
    }
}

void Arena::add_plataform(float x, float y, float width, float height) {
    objects::Box plataform({x, y, 0}, {width, height, m_dimensions.z}, 16);
    plataform.set_color(1, 1, 1, 1);
    plataform.set_specular({0.8, 0.8, 0.8, 1});
    plataform.set_texture("block");
    m_plataforms.push_back(plataform);
}

void Arena::display() {

    for (int i = 0; i < 8; i++) {
        m_lights[i].set_active(m_active_lights[i]);
        m_lights[i].set_show_axes(m_show_axes);
        m_lights[i].display();
    }

    v4f specular = {0.2, 0.2, 0.2, 0.2};
    float shininess = 0;
    int subdivisions = 1;

    objects::Plane floor(
        {0, 0, m_dimensions.z},
        {m_dimensions.x, 0, m_dimensions.z},
        {m_dimensions.x, 0, 0},
        {0, 0, 0},
        subdivisions
    );
    floor.set_color(1, 1, 1, 1);
    floor.set_show_axes(m_show_axes);
    floor.set_specular(specular);
    floor.set_shininess(shininess);
    floor.set_texture("floor");
    floor.display();

    objects::Plane left_wall(
        {0, 0, 0},
        {m_dimensions.x, 0, 0},
        {m_dimensions.x, m_dimensions.y, 0},
        {0, m_dimensions.y, 0},
        subdivisions
    );
    left_wall.set_color(1, 1, 1, 1);
    left_wall.set_show_axes(m_show_axes);
    left_wall.set_specular(specular);
    left_wall.set_shininess(shininess);
    left_wall.set_texture("wall");
    left_wall.display();

    objects::Plane right_wall(
        {0, m_dimensions.y, m_dimensions.z},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        {m_dimensions.x, 0, m_dimensions.z},
        {0, 0, m_dimensions.z},
        subdivisions
    );
    right_wall.set_color(1, 1, 1, 1);
    right_wall.set_show_axes(m_show_axes);
    right_wall.set_specular(specular);
    right_wall.set_shininess(shininess);
    right_wall.set_texture("wall");
    right_wall.display();

    objects::Plane back_wall(
        {0, 0, 0},
        {0, m_dimensions.y, 0},
        {0, m_dimensions.y, m_dimensions.z},
        {0, 0, m_dimensions.z},
        subdivisions
    );
    back_wall.set_color(1, 1, 1, 1);
    back_wall.set_show_axes(m_show_axes);
    back_wall.set_specular(specular);
    back_wall.set_shininess(shininess);
    back_wall.display();

    objects::Plane front_wall(
        {m_dimensions.x, 0, 0},
        {m_dimensions.x, 0, m_dimensions.z},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        {m_dimensions.x, m_dimensions.y, 0},
        subdivisions
    );
    front_wall.set_color(1, 1, 1, 1);
    front_wall.set_show_axes(m_show_axes);
    front_wall.set_specular(specular);
    front_wall.set_shininess(shininess);
    front_wall.display();

    objects::Plane ceiling(
        {0, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        {0, m_dimensions.y, m_dimensions.z},
        subdivisions
    );
    ceiling.set_color(1, 1, 1, 1);
    ceiling.set_show_axes(m_show_axes);
    ceiling.set_specular(specular);
    ceiling.set_shininess(shininess);
    ceiling.set_texture("floor");
    ceiling.display();

    for (auto plataform : m_plataforms) {
        plataform.set_show_axes(m_show_axes);
        plataform.display();
    }
}

