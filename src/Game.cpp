#include "Game.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <tinyxml2.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Texture.h"
#include "Objects/Text.h"

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b

Game::Game() {

    for (int i = 0; i < 256; i++) m_key_state[i] = false;
    for (int i = 0; i < 3; i++) m_mouse_state[i] = false;
    m_show_axes = false;
    m_textures = std::unordered_map<std::string, Texture*>();
    m_enemies = std::vector<objects::Enemy>();

    m_last_x = m_last_y = 0;
    m_last_phi = m_last_theta = 0;
    m_warping = false;
    m_phi = m_theta = 0;
    m_clock = 0;
    m_free_camera = false;
    m_orbital_zoom = 1.0;
    m_lights_on = true;
    m_invincible_player = false;

    m_dt = 0;
    m_gravity = 200;
}

void glut_display() {
    game.display();
    glutSwapBuffers();
}

void glut_mouse_click(int button, int state, int x, int y) {
    game.handle_mouse_click(button, state, x, y);
    glutPostRedisplay();
}

void glut_mouse_move(int x, int y) {
    game.handle_mouse_move(x, y);
    glutPostRedisplay();
}

void glut_key_down(unsigned char key, int x, int y) {
    game.handle_key_down(key, x, y);
    glutPostRedisplay();
}

void glut_key_up(unsigned char key, int x, int y) {
    game.handle_key_up(key, x, y);
    glutPostRedisplay();
}

float time_since_last_frame() {
    static int previous = glutGet(GLUT_ELAPSED_TIME);
    int current, dt;
    current = glutGet(GLUT_ELAPSED_TIME);
    dt = current - previous;
    previous = current;

    return (float)dt/1000;
}

void glut_idle() {
    float dt = time_since_last_frame();
    game.update(dt);
    glutPostRedisplay();
}

void Game::load_arena(std::string path) {
    tinyxml2::XMLDocument level;
    level.LoadFile(path.c_str());

    auto player_info = level.FirstChild()->FirstChildElement("circle");
    for (; player_info != nullptr; player_info = player_info->NextSiblingElement("circle")) {
        if (std::string(player_info->Attribute("fill")) == "green") break;
    }

    float x = std::stod(player_info->Attribute("cx"));
    float y = std::stod(player_info->Attribute("cy"));
    float z = 0;

    float height = 2*std::stod(player_info->Attribute("r"));

    auto arena_info = level.FirstChild()->FirstChildElement("rect");
    for (; arena_info != nullptr; arena_info = arena_info->NextSiblingElement("rect")) {
        if (std::string(arena_info->Attribute("fill")) == "blue") break;
    }

    float base_x = std::stod(arena_info->Attribute("x"));
    float base_y = std::stod(arena_info->Attribute("y"));
    float arena_width = std::stod(arena_info->Attribute("width"));
    float arena_height = std::stod(arena_info->Attribute("height"));

    m_player = objects::Player(x-base_x, arena_height - ( y - base_y) - height/2, arena_height/4, height, height/6, this);
    m_arena = Arena(arena_width, arena_height, this);

    // camera
    v3f camera_position = {
        m_player.center().x,
        m_player.center().y + 1,
        m_player.center().z + 3*height
    };
    m_camera = objects::Camera(camera_position);

    // plataforms
    for (auto rect = level.FirstChild()->FirstChildElement("rect"); rect != nullptr; rect = rect->NextSiblingElement("rect")) {
        if (std::string(rect->Attribute("fill")) == "blue") continue;
        float x = std::stod(rect->Attribute("x")) - base_x;
        float y = arena_height - (std::stod(rect->Attribute("y")) - base_y);
        float width = std::stod(rect->Attribute("width"));
        float height = std::stod(rect->Attribute("height"));
        m_arena.add_plataform(x, y-height, width, height);
    }

    // enemies
    for (auto circ = level.FirstChild()->FirstChildElement("circle"); circ != nullptr; circ = circ->NextSiblingElement("circle")) {
        if (std::string(circ->Attribute("fill")) == "green") continue;
        float x = std::stod(circ->Attribute("cx")) - base_x;
        float y = arena_height - (std::stod(circ->Attribute("cy")) - base_y);
        float height = 2*std::stod(circ->Attribute("r"));
        float radius = height/6;
        this->add_enemy(x, y, arena_height/4, height, radius);
    }
}

void Game::add_enemy(float x, float y, float z, float height, float radius) {
    objects::Enemy enemy(x, y, z, height, radius, this);
    enemy.set_max_velocity(enemy.height()*2);
    enemy.set_velocity((v3f){(float)(rand()%10), 0, (float)(rand()%10)}.normalize()*enemy.max_velocity());
    m_enemies.push_back(enemy);
}

void Game::load_texture(std::string path, std::string name, bool box) {
    auto texture = new Texture(name, path, box);
    m_textures.insert({name, texture});
}

Texture* Game::get_texture(std::string name) {
    return m_textures[name];
}

void Game::load(std::string level_file_path) {
    load_texture("assets/wall.bmp", "wall");
    load_texture("assets/floor.bmp", "floor");
    load_texture("assets/block.bmp", "block");
    load_texture("assets/light.bmp", "light");
    load_texture("assets/enemy_head.bmp", "enemy_head", true);
    load_texture("assets/body.bmp", "body", true);
    load_texture("assets/upper_leg.bmp", "upper_leg", true);
    load_texture("assets/lower_leg.bmp", "lower_leg", true);
    load_texture("assets/arms.bmp", "arms", true);
    load_texture("assets/head.bmp", "head", true);
    load_arena(level_file_path);
}

void Game::reset() {
    for (int i = 0; i < m_enemies.size(); i++) {
        m_enemies[i].set_alive(true);
        m_enemies[i].set_position(m_enemies[i].initial_position());
    }
    m_shots.clear();
    m_player.set_alive(true);
    m_player.set_position(m_player.initial_position());
    m_player.set_won(false);
}

void Game::init(int window_width, int window_height, std::string window_name, int* argc, char** argv) {
    m_window_width = window_width;
    m_window_height = window_height;

    // init window
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow(window_name.c_str());

    // enable flags
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, (GLsizei)m_window_width, (GLsizei)m_window_height);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    float global_amb[] = { 0.1f, 0.1f, 0.1f, 1.f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_amb);
    glutSetCursor(GLUT_CURSOR_NONE);


    // load data
    std::string level_file_path = argv[1];
    load(level_file_path);

    // camera
    m_camera.init();

    // bind functions
    glutDisplayFunc(glut_display);
    glutIdleFunc(glut_idle);
    glutMotionFunc(glut_mouse_move);
    glutPassiveMotionFunc(glut_mouse_move);
    glutMouseFunc(glut_mouse_click);
    glutKeyboardFunc(glut_key_down);
    glutKeyboardUpFunc(glut_key_up);
}

void Game::run() {
    glutMainLoop();
}

void Game::handle_mouse_click(int button, int state, int x, int y) {
    m_mouse_state[button] = !state;

    if (button == 0 && state == 0 && m_player.alive()) {
        auto shot = m_player.shoot();
        shot.set_max_velocity(m_player.max_velocity()*3);
        m_shots.push_back(shot);
    }
}

float clamp(float val, float low, float high) {
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

void Game::handle_mouse_move(int x, int y) {
    if (m_warping) {
        m_warping = false;
        return;
    }

    float dphi = -2*M_PI*((float)(x-m_window_width/2)/m_window_width);
    float dtheta = M_PI*((float)(y-m_window_height/2)/m_window_height);

    m_phi = m_last_phi + dphi;
    m_theta = m_last_theta + dtheta;
    m_theta = clamp(m_theta, -M_PI/3, M_PI/3);

    m_last_phi = m_phi;
    m_last_theta = m_theta;

    m_camera.set_direction(
        -sin(m_phi - M_PI_2)*cos(m_theta),
        -sin(m_theta),
        -cos(m_theta)*cos(m_phi - M_PI_2)
    );

    if (!m_free_camera) {
        v3f player_direction = { m_camera.direction().x, 0, m_camera.direction().z };
        m_player.set_direction(player_direction.normalize());
        m_player.set_aim(m_camera.direction());
    }

    m_warping = true;
    glutWarpPointer(m_window_width/2, m_window_height/2);
}

void Game::handle_key_down(unsigned char key, int x, int y) {
    key = tolower(key);
    m_key_state[key] = true;

    if (key == 'x') {
        auto new_mode = (objects::Camera::camera_mode)((m_camera.mode() + 1)%2);
        m_camera.set_mode(new_mode);

        if (new_mode == objects::Camera::orbital) {
            float rho = 3*m_player.height();
            m_camera.set_position(
                rho*sin(m_phi - M_PI_2)*cos(m_theta) + m_player.center().x,
                rho*sin(m_theta) + m_player.center().y,
                rho*cos(m_theta)*cos(m_phi - M_PI_2) + m_player.center().z
            );
        } else {
            m_free_camera = false;
        }
    }

    if (key == 'u') {
        std::cout << "Camera position: " << m_camera.position() << '\n';
        std::cout << "Camera direction: " << m_camera.direction() << '\n';
    }

    if (key == 'p') {
        m_show_axes = !m_show_axes;
    }
    if (key == 'n') {
        m_lights_on = !m_lights_on;
        m_player.set_flashlight_on(!m_lights_on);
    }

    if (key == 't') {
        if (m_camera.mode() == objects::Camera::orbital) {
            m_free_camera = !m_free_camera;
        }
    }

    if (key == 'r') {
        this->reset();
    }

    if (key == '+') {
        m_orbital_zoom += 0.05;
        m_orbital_zoom = min(m_orbital_zoom, 2.0);
    }

    if (key == '-') {
        m_orbital_zoom -= 0.05;
        m_orbital_zoom = max(m_orbital_zoom, 0.5);
    }

    if (key == 'i') {
        m_invincible_player = !m_invincible_player;
    }
}

void Game::handle_key_up(unsigned char key, int x, int y) {
    m_key_state[tolower(key)] = false;
}

void Game::handle_key_state() {
    v3f velocity = {0, 0, 0};
    float jump_velocity = m_player.velocity().y;

    v3f front  = m_camera.direction().normalize();
    front.y = 0;
    if (m_key_state['w']) {
        velocity = velocity + front;
    }

    if (m_key_state['s']) {
        v3f back = {-front.x, -front.y, -front.z};
        velocity = velocity + back;
    }

    if (m_key_state['d']) {
        v3f right = {-front.z, -front.y, front.x};
        velocity = velocity + right;
    }

    if (m_key_state['a']) {
        v3f left = {front.z, front.y, -front.x};
        velocity = velocity + left;
    }

    if (m_key_state[' ']) {
        if (m_player.grounded()) {
            m_player.set_grounded(false);
            jump_velocity += m_player.jump_velocity();
        }
    } else {
        if (!m_player.grounded() && jump_velocity > 0) {
            jump_velocity = jump_velocity/2;
        }
    }

    if (velocity != (v3f){0, 0, 0}) {
        velocity = velocity.normalize();
    }

    m_player.set_velocity(velocity*m_player.max_velocity());
    m_player.set_velocity_y(jump_velocity);
}

void Game::handle_mouse_state() {
    if (m_mouse_state[GLUT_RIGHT_BUTTON] && m_camera.mode() == objects::Camera::first_person) {
        m_camera.set_mode(objects::Camera::aiming);
    } else if(!m_mouse_state[GLUT_RIGHT_BUTTON] && m_camera.mode() == objects::Camera::aiming) {
        m_camera.set_mode(objects::Camera::first_person);
    }
}

void Game::kill_player() {
    if (m_invincible_player) return;

    m_player.set_alive(false);
}

void Game::handle_player_movement() {
    m_player.update_current_animation(m_dt);

    m_player.increase_velocity(0, -m_gravity*m_dt, 0);

    // arena collisions
    m_player.arena_collision(m_arena, m_dt);

    // plataform collisions
    for (auto plataform : m_arena.plataforms()) {
        m_player.plataform_collision(plataform, m_dt);
    }

    // enemy collisions
    for (int i = 0; i < m_enemies.size(); i++) {
        if (m_enemies[i].alive())
            m_player.enemy_collision(&m_enemies[i], m_dt);
    }

    if (m_player.velocity().y != 0) m_player.set_grounded(false);

    if (!m_player.grounded()) {
        m_player.set_current_animation("jumping");
    } else if (m_player.velocity().x != 0 || m_player.velocity().z != 0) {
        if (m_player.velocity().dot(m_player.direction()) < 0)
            m_player.set_current_animation("backwards");
        else
            m_player.set_current_animation("running");
    } else {
        m_player.set_current_animation("still");
    }

    v3f displacement = m_player.velocity() * m_dt;

    m_player.translate(displacement);
    m_camera.translate(displacement);
}

void Game::handle_enemy_movement(objects::Enemy* enemy) {
    enemy->update_current_animation(m_dt);

    // choose a random direction
    if (enemy->clock() <= 0) {
        float random = (float)rand()/RAND_MAX;
        if (random < 0.25) {
            v3f velocity = (v3f){ (float)(rand()%10+1), 0, (float)(rand()%10+1) }.normalize();
            enemy->set_velocity(velocity * enemy->max_velocity());
        } else if (random < 0.50) {
            enemy->set_velocity_x(0);
            enemy->set_velocity_y(0);
        } else {
            auto shot = enemy->shoot(m_player.center());
            shot.set_max_velocity(m_player.max_velocity()*3);
            m_shots.push_back(shot);
        }
        enemy->set_clock((float)(rand()%3 + 3));
    }

    enemy->increase_velocity(0, -m_gravity*m_dt, 0);

    // arena collisions
    enemy->arena_collision(m_arena, m_dt);

    // plataforms collisions
    for (auto plataform : m_arena.plataforms()) {
        enemy->plataform_collision(plataform, m_dt);
    }

    if (enemy->velocity().y != 0)
        enemy->set_grounded(true);

    v3f displacement = enemy->velocity() * m_dt;

    enemy->translate(displacement);

    if (enemy->velocity().x != 0 || enemy->velocity().z != 0) {
        if (enemy->velocity().dot(enemy->direction()) < 0)
            enemy->set_current_animation("backwards");
        else
            enemy->set_current_animation("running");
    } else {
        enemy->set_current_animation("still");
    }

    // look at player
    v3f direction = (m_player.center() - enemy->center());
    enemy->set_direction((v3f){direction.x, 0, direction.z}.normalize());
    enemy->set_aim(direction.normalize());

    // limit xy rotation to 60 degrees
    if (enemy->aim().dot(enemy->direction()) <= 0.5) {
        float s = sin(M_PI/3);
        if (enemy->aim().y < 0) s = -s;
        v3f aim = { (enemy->direction()*0.5).x, s, (enemy->direction()*0.5).z };
        enemy->set_aim(aim.normalize());
    }
}

bool Game::handle_shot_movement(objects::Shot* shot) {

    // arena
    if (shot->arena_collision(m_arena, m_dt)) return true;

    // enemies
    for (int i = 0; i < m_enemies.size(); i++) {
        if (!m_enemies[i].alive()) continue;
        if (shot->character_collision(m_enemies[i], m_dt)) {
            m_enemies[i].set_alive(false);
            return true;
        }
    }

    // player
    if (m_player.alive() && shot->character_collision(m_player, m_dt)) {
        kill_player();
        return true;
    }

    // plataforms
    for (auto plataform : m_arena.plataforms()) {
        if (shot->plataform_collision(plataform, m_dt)) {
            return true;
        }
    }

    v3f displacement = shot->direction() * shot->max_velocity() * m_dt;
    shot->translate(displacement);
    return false;
}

void Game::update(float dt) {
    m_dt = dt;
    handle_key_state();
    handle_mouse_state();

    if (m_player.alive() && !m_player.won()) {
        handle_player_movement();
    }

    for (int i = 0; i < m_enemies.size(); i++) {
        if (!m_enemies[i].alive()) continue;

        m_enemies[i].increase_clock(-dt);
        handle_enemy_movement(&m_enemies[i]);
    }
    for (auto it = m_shots.begin(); it != m_shots.end();) {
        auto shot_ptr = &*it;
        if (handle_shot_movement(shot_ptr)) {
            it = m_shots.erase(it);
        } else {
            it++;
        }
    }
}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!m_player.alive() && !m_player.won()) {
        objects::Text text(m_window_width/2, m_window_height/2, 0, "You Lose :(");
        text.set_screen(m_window_width, m_window_height);
        text.set_color(1, 0, 0, 1);
        text.display();
    } else if (m_player.won()) {
        objects::Text text(m_window_width/2, m_window_height/2, 0, "You Win! :)");
        text.set_screen(m_window_width, m_window_height);
        text.set_color(0, 1, 0, 1);
        text.display();
    }

    if (m_camera.mode() == objects::Camera::orbital) {
        // put in orbit around player
        float rho = 3*m_player.height()*m_orbital_zoom;
        m_camera.set_position(
            rho*sin(m_phi - M_PI_2)*cos(m_theta) + m_player.center().x,
            rho*sin(m_theta) + m_player.center().y + m_player.height()/2,
            rho*cos(m_theta)*cos(m_phi - M_PI_2) + m_player.center().z
        );
    }
    else if (m_camera.mode() == objects::Camera::first_person) {
        m_camera.set_position(m_player.center() + (v3f){0, m_player.height()/2, 0});
    }
    else if (m_camera.mode() == objects::Camera::aiming) {
        m_camera.set_position(m_player.gun_position());
    }
    m_camera.look_at(m_camera.position() + m_camera.direction());

    for (auto enemy : m_enemies) {
        if (enemy.alive())
            enemy.display();
    }

    for (auto shot : m_shots) {
        shot.display();
    }

    m_arena.set_lights_on(m_lights_on);
    m_arena.set_show_axes(m_show_axes);
    m_arena.display();

    if (m_player.alive() && !m_player.won()) {
        m_player.display();
        m_player.set_show_axes(m_show_axes);
    }

}