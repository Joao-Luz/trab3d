#include "Arena.h"

#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Game.h"
#include "Objects/Plane.h"
#include <random>

Arena::Arena(float width, float height, Game* game) : m_game(game) {
    m_dimensions = {width, height, height/2};
    for (int i = 0; i < 3; i++) {
        float x = i*m_dimensions.x/3 + m_dimensions.x/6;
        float y = m_dimensions.y-8;
        objects::LightSource light_left(
            {x, y, 0.5, 1.0},
            {0.7, 0.7, 0.7, 1}, // ambient
            {0.2f, 0.2f, 0.2f, 0.2f}, // diffuse
            {1.0f, 1.0f, 1.0f, 1.0f}, // specular
            2*i
        );
        m_lights[2*i] = light_left;
        
        objects::LightSource light_right(
            {x, y, m_dimensions.z-0.5f, 1.0},
            {0.7, 0.7, 0.7, 1}, // ambient
            {0.2f, 0.2f, 0.2f, 0.2f}, // diffuse
            {1.0f, 1.0f, 1.0f, 1.0f}, // specular
            2*i+1
        );
        m_lights[2*i+1] = light_right;
    }
}

void Arena::add_plataform(float x, float y, float width, float height) {
    objects::Box plataform({x, y, 0}, {width, height, m_dimensions.z}, 8);
    plataform.set_color(1.0f, 1.0f, 1.0f, 1.0f);
    plataform.set_specular({1.0f, 1.0f, 1.0f, 1.0f});
    plataform.set_texture(m_game->get_texture("block"));
    m_plataforms.push_back(plataform);
}

void Arena::display() {

    for (int i = 0; i < 6; i++) {
        m_lights[i].set_active(m_lights_on);
        m_lights[i].set_show_axes(m_show_axes);
        m_lights[i].display();

        objects::Box lamp(m_lights[i].position() - (v3f){5.5, 8, 0.125}, {11, 16, 0.25}, 8);
        lamp.set_show_axes(m_show_axes);
        if (m_lights_on)
            lamp.set_emission(1, 1, 0.7, 1);
        lamp.set_texture(m_game->get_texture("light"));
        lamp.display();

    }

    v4f specular = {0.2, 0.2, 0.2, 0.2};
    float shininess = 1;

    float ratio = m_dimensions.x/m_dimensions.z;
    int sub_x = floor(m_dimensions.x/ratio)/2;
    int sub_y = floor(m_dimensions.z/ratio)/2;

    objects::Plane ground(
        {0, 0, m_dimensions.z},
        {m_dimensions.x, 0, m_dimensions.z},
        {m_dimensions.x, 0, 0},
        {0, 0, 0},
        1
    );
    ground.set_subdivisions_x(sub_x);
    ground.set_subdivisions_y(sub_y);
    ground.set_color(1, 1, 1, 1);
    ground.set_show_axes(m_show_axes);
    ground.set_specular(specular);
    ground.set_shininess(shininess);
    ground.set_texture(m_game->get_texture("floor"));
    ground.display();

    ratio = m_dimensions.x/m_dimensions.y;
    sub_x = floor(m_dimensions.x/ratio)/2;
    sub_y = floor(m_dimensions.y/ratio)/2;

    objects::Plane ceiling(
        {0, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, 0},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        {0, m_dimensions.y, m_dimensions.z},
        1
    );
    ceiling.set_subdivisions_x(sub_x);
    ceiling.set_subdivisions_y(sub_y);
    ceiling.set_color(1, 1, 1, 1);
    ceiling.set_show_axes(m_show_axes);
    ceiling.set_specular(specular);
    ceiling.set_shininess(shininess);
    ceiling.set_texture(m_game->get_texture("floor"));
    ceiling.display();

    objects::Plane left_wall(
        {0, 0, 0},
        {m_dimensions.x, 0, 0},
        {m_dimensions.x, m_dimensions.y, 0},
        {0, m_dimensions.y, 0},
        1
    );
    left_wall.set_subdivisions_x(sub_x);
    left_wall.set_subdivisions_y(sub_y);
    left_wall.set_color(1, 1, 1, 1);
    left_wall.set_show_axes(m_show_axes);
    left_wall.set_specular(specular);
    left_wall.set_shininess(shininess);
    left_wall.set_texture(m_game->get_texture("wall"));
    left_wall.display();

    objects::Plane right_wall(
        {m_dimensions.x, 0, m_dimensions.z},
        {0, 0, m_dimensions.z},
        {0, m_dimensions.y, m_dimensions.z},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        1
    );
    right_wall.set_subdivisions_x(sub_x);
    right_wall.set_subdivisions_y(sub_y);
    right_wall.set_color(1, 1, 1, 1);
    right_wall.set_show_axes(m_show_axes);
    right_wall.set_specular(specular);
    right_wall.set_shininess(shininess);
    right_wall.set_texture(m_game->get_texture("wall"));
    right_wall.display();

    ratio = m_dimensions.y/m_dimensions.z;
    sub_x = floor(m_dimensions.z/ratio)/2;
    sub_y = floor(m_dimensions.y/ratio)/2;
    objects::Plane back_wall(
        {0, 0, m_dimensions.z},
        {0, 0, 0},
        {0, m_dimensions.y, 0},
        {0, m_dimensions.y, m_dimensions.z},
        1
    );
    back_wall.set_subdivisions_x(sub_x);
    back_wall.set_subdivisions_y(sub_y);
    back_wall.set_color(1, 1, 1, 1);
    back_wall.set_show_axes(m_show_axes);
    back_wall.set_specular(specular);
    back_wall.set_shininess(shininess);
    back_wall.set_texture(m_game->get_texture("wall"));
    back_wall.display();

    objects::Plane front_wall(
        {m_dimensions.x, 0, 0},
        {m_dimensions.x, 0, m_dimensions.z},
        {m_dimensions.x, m_dimensions.y, m_dimensions.z},
        {m_dimensions.x, m_dimensions.y, 0},
        1
    );
    front_wall.set_subdivisions_x(sub_x);
    front_wall.set_subdivisions_y(sub_y);
    front_wall.set_color(1, 1, 1, 1);
    front_wall.set_show_axes(m_show_axes);
    front_wall.set_specular(specular);
    front_wall.set_shininess(shininess);
    front_wall.set_texture(m_game->get_texture("wall"));
    front_wall.display();

    for (auto plataform : m_plataforms) {
        plataform.set_show_axes(m_show_axes);
        plataform.display();
    }
}

