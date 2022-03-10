#include "Game.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <tinyxml2.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Textures.h"

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b

Game::Game() {
    std::cout << "Init\n"; 

    for (int i = 0; i < 256; i++) m_key_state[i] = false;
    for (int i = 0; i < 8; i++) m_active_lights[i] = false;
    m_show_axes = false;
    m_textures = std::unordered_map<std::string, int>();
    m_enemies = std::vector<objects::Character>();

    m_last_x = m_last_y = 0;
    m_last_phi = m_last_theta = 0;
    m_phi = m_theta = 0;
    m_yz = m_xz = 0;
    m_clock = 0;

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

    m_player = objects::Character(x-base_x, arena_height - ( y - base_y) - height/2, arena_height/4, height, height/4);
    m_arena = Arena(arena_width, arena_height, this);

    for (int i = 0; i < 8; i++) {
        m_active_lights[i] = true;
    }

    // camera
    v3f camera_position = {
        m_player.center().x,
        m_player.center().y + 1,
        m_player.center().z + 3*height
    };
    m_camera = objects::Camera(camera_position);
    m_camera.set_direction({1, 0, 0});

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
        float radius = height/4;
        this->add_enemy(x, y, arena_height/4, height, radius);
    }
}

void Game::add_enemy(float x, float y, float z, float height, float radius) {
    objects::Character enemy(x, y, z, height, radius);
    enemy.set_max_velocity(enemy.height()*2);
    enemy.set_velocity((v3f){(float)(rand()%10), 0, (float)(rand()%10)}.normalize()*enemy.max_velocity());
    m_enemies.push_back(enemy);
}

void Game::load_texture(std::string path, std::string name) {
    int id = Textures::load_texture(path, name);
    m_textures.insert({name, id});
}

int Game::get_texture(std::string name) {
    int id = m_textures[name];
    return id;
}

void Game::load(std::string level_file_path) {
    load_texture("assets/wall.bmp", "wall");
    load_texture("assets/floor.bmp", "floor");
    load_texture("assets/block.bmp", "block");
    load_texture("assets/light.bmp", "light");
    load_arena(level_file_path);
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


    // load data
    std::string level_file_path = argv[1];
    load(level_file_path);

    // camera
    m_camera.init();

    // bind functions
    glutDisplayFunc(glut_display);
    glutIdleFunc(glut_idle);
    glutMotionFunc(glut_mouse_move);
    glutMouseFunc(glut_mouse_click);
    glutKeyboardFunc(glut_key_down);
    glutKeyboardUpFunc(glut_key_up);


}

void Game::run() {
    glutMainLoop();
}

void Game::handle_mouse_click(int button, int state, int x, int y) {
    if (button == 0 && state == 0) {
        m_last_x = x;
        m_last_y = y;
    }

    else if (button == 0 && state == 1) {
        m_last_phi = m_phi;
        m_last_theta = m_theta;
    }
}

float clamp(float val, float low, float high) {
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

void Game::handle_mouse_move(int x, int y) {
    float dphi = -2*M_PI*((float)(x - m_last_x)/m_window_width);
    float dtheta = M_PI*((float)(y - m_last_y)/m_window_height);

    m_phi = m_last_phi + dphi;
    m_theta = m_last_theta + dtheta;
    m_theta = clamp(m_theta, -M_PI/3, M_PI/3);

    m_yz = 180*(m_theta)/M_PI;
    m_xz = -180*(m_phi)/M_PI;

    float rho = 3*m_player.height();

    if (m_camera.mode() == objects::Camera::orbital) {

        m_camera.set_position(
            rho*sin(m_phi)*cos(m_theta) + m_player.center().x,
            rho*sin(m_theta) + m_player.center().y,
            rho*cos(m_theta)*cos(m_phi) + m_player.center().z
        );

        m_camera.set_direction(m_player.center() - m_camera.position());
    }

    else if (m_camera.mode() == objects::Camera::first_person) {
        m_camera.set_angle(m_yz, m_xz, 0);

        m_camera.set_direction(
            -sin(m_phi)*cos(m_theta),
            -sin(m_theta),
            -cos(m_theta)*cos(m_phi)
        );
    }
}

void Game::handle_key_down(unsigned char key, int x, int y) {
    key = tolower(key);
    m_key_state[key] = true;

    if (key == 'x') {
        auto new_mode = (objects::Camera::camera_mode)((m_camera.mode() + 1)%2);
        m_camera.set_mode(new_mode);

        if (new_mode == objects::Camera::first_person) {
            m_camera.set_angle(m_yz, m_xz, 0);
            v3f new_direction = {m_camera.direction().x, 0, -m_camera.direction().z};
            m_camera.set_direction(new_direction);
        } else {
            float rho = 3*m_player.height();
            m_camera.set_position(
                rho*sin(m_phi)*cos(m_theta) + m_player.center().x,
                rho*sin(m_theta) + m_player.center().y,
                rho*cos(m_theta)*cos(m_phi) + m_player.center().z
            );
            m_camera.set_direction(m_player.center() - m_camera.position());
        }
    }

    if (key == 'u') {
        std::cout << m_camera.position().x << " " << m_camera.position().y << " " << m_camera.position().z << '\n';
        std::cout << m_camera.center().x << " " << m_camera.center().y << " " << m_camera.center().z << '\n';
    }

    if (key == 'p') {
        m_show_axes = !m_show_axes;
    }
    if (key >= '0' && key <= '9') {
        int light_id = key-'0'-1;
        m_active_lights[light_id] = !m_active_lights[light_id]; 
    }
    m_arena.set_active_lights(m_active_lights);
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

void Game::enemy_plataform_collision(objects::Character* enemy) {
    v3f next_position = enemy->center() + enemy->velocity()*m_dt;

    // handle walls, floor and ceiling collision
    if ((next_position.z - enemy->radius()) < 0) {
        enemy->set_velocity_z(-enemy->velocity().z);
        enemy->set_center_z(enemy->radius());
    }

    if ((next_position.z + enemy->radius()) > m_arena.length()) {
        enemy->set_velocity_z(-enemy->velocity().z);
        enemy->set_center_z(m_arena.length() - enemy->radius());
    }

    if ((next_position.x - enemy->radius()) < 0) {
        enemy->set_velocity_x(-enemy->velocity().x);
        enemy->set_center_x(enemy->radius());
    }

    if ((next_position.x + enemy->radius()) > m_arena.width()) {
        enemy->set_velocity_x(-enemy->velocity().x);
        enemy->set_center_x(m_arena.width() - enemy->radius());
    }

    if ((next_position.y - enemy->height()/2) < 0) {
        enemy->set_velocity_y(0);
        enemy->set_center_y(enemy->height()/2);
        enemy->set_grounded(true);
    }

    if ((next_position.y + enemy->height()/2) > m_arena.height()) {
        enemy->set_velocity_y(0);
        enemy->set_center_y(m_arena.height() - enemy->height()/2);
    }

    bool collided_down = false;
    for (auto plataform : m_arena.plataforms()) {
        v3f center = plataform.center();
        float width = plataform.scale().x;
        float height = plataform.scale().y;
        float radius = enemy->radius();
        float player_h = enemy->height();
        if ((next_position.x + radius) > (center.x - width/2) && 
            (next_position.x - radius) < (center.x + width/2) &&
            (next_position.y + player_h/2) > (center.y - height/2) &&
            (next_position.y - player_h/2) < (center.y + height/2)) {
            
            // front
            if ((enemy->center().x + radius) < (plataform.center().x - width/2)) {
                enemy->set_velocity_x(-enemy->velocity().x);
            } else 

            // back
            if ((enemy->center().x - radius) > (plataform.center().x + width/2)) {
                enemy->set_velocity_x(-enemy->velocity().x);
            } else

            // top
            if ((enemy->center().y + player_h/2) < (plataform.center().y - height/2)) {
                enemy->set_velocity_y(0);
            } else

            // bottom
            if ((enemy->center().y - player_h/2) > (plataform.center().y + height/2)) {
                enemy->set_velocity_y(0);
                enemy->set_grounded(true);
                if ((enemy->center().x + enemy->radius()) >= (center.x + width/2) ||
                    (enemy->center().x - enemy->radius()) <= (center.x - width/2)) {
                    enemy->set_velocity_x(-enemy->velocity().x);
                }
            }
        }
    }
}

void Game::player_plataform_collision() {
    v3f next_position = m_player.center() + m_player.velocity()*m_dt;

    // handle walls, floor and ceiling collision
    if ((next_position.z - m_player.radius()) < 0) {
        m_player.set_velocity_z(0);
        m_player.set_center_z(m_player.radius());
    }

    if ((next_position.z + m_player.radius()) > m_arena.length()) {
        m_player.set_velocity_z(0);
        m_player.set_center_z(m_arena.length() - m_player.radius());
    }

    if ((next_position.x - m_player.radius()) < 0) {
        m_player.set_velocity_x(0);
        m_player.set_center_x(m_player.radius());
    }

    if ((next_position.x + m_player.radius()) > m_arena.width()) {
        m_player.set_velocity_x(0);
        m_player.set_center_x(m_arena.width() - m_player.radius());
    }

    if ((next_position.y - m_player.height()/2) < 0) {
        m_player.set_velocity_y(0);
        m_player.set_center_y(m_player.height()/2);
        m_player.set_grounded(true);
    }

    if ((next_position.y + m_player.height()/2) > m_arena.height()) {
        m_player.set_velocity_y(0);
        m_player.set_center_y(m_arena.height() - m_player.height()/2);
    }

    for (auto plataform : m_arena.plataforms()) {
        v3f center = plataform.center();
        float width = plataform.scale().x;
        float height = plataform.scale().y;
        float radius = m_player.radius();
        float player_h = m_player.height();
        if ((next_position.x + radius) > (center.x - width/2) && 
            (next_position.x - radius) < (center.x + width/2) &&
            (next_position.y + player_h/2) > (center.y - height/2) &&
            (next_position.y - player_h/2) < (center.y + height/2)) {
            
            // front
            if ((m_player.center().x + radius) < (plataform.center().x - width/2)) {
                m_player.set_velocity_x(0);
            } else 

            // back
            if ((m_player.center().x - radius) > (plataform.center().x + width/2)) {
                m_player.set_velocity_x(0);
            } else

            // top
            if ((m_player.center().y + player_h/2) < (plataform.center().y - height/2)) {
                m_player.set_velocity_y(0);
            } else

            // bottom
            if ((m_player.center().y - player_h/2) > (plataform.center().y + height/2)) {
                m_player.set_velocity_y(0);
                m_player.set_grounded(true);
            }
        }
    }
}

void Game::player_enemy_collision(objects::Character* enemy) {
    v3f next_player_position = m_player.center() + (m_player.velocity())*m_dt;
    v3f next_enemy_position = enemy->center() + (enemy->velocity())*m_dt;

    if ((next_player_position.y + m_player.height()/2) > (next_enemy_position.y - enemy->height()/2) &&
        (next_player_position.y - m_player.height()/2) < (next_enemy_position.y + enemy->height()/2)) {
        
        next_player_position.y = 0;
        next_enemy_position.y = 0;
        float distance = (next_enemy_position - next_player_position).norm();

        if (distance < (m_player.radius() + enemy->radius())) {
            // bottom top
            if ((m_player.center().y - m_player.height()/2) > (enemy->center().y + enemy->height()/2)) {
                m_player.set_velocity_y(0);
                m_player.set_grounded(true);
            // sides
            } else {

                float a = m_player.velocity().x * enemy->velocity().x;
                a = a > 0 ? 1 : -1;
                float b = m_player.velocity().z * enemy->velocity().z;
                b = b > 0 ? 1 : -1;
                enemy->set_velocity_x(a*enemy->velocity().x);
                enemy->set_velocity_z(b*enemy->velocity().z);

                m_player.set_velocity_x(0);
                m_player.set_velocity_z(0);
            }
        }
    }
}

void Game::handle_player_movement() {
    m_player.increase_velocity(0, -m_gravity*m_dt, 0);

    player_plataform_collision();
    for (int i = 0; i < m_enemies.size(); i++) {
        player_enemy_collision(&m_enemies[i]);
    }

    if (m_player.velocity().y != 0) m_player.set_grounded(false);

    v3f displacement = m_player.velocity() * m_dt;

    m_player.translate(displacement);
    m_camera.translate(displacement);
}

void Game::handle_enemy_movement(objects::Character* enemy) {
    
    // choose a random direction
    if (enemy->clock() <= 0) {
        if ((float)rand()/RAND_MAX < 0.6) {
            v3f velocity = (v3f){ (float)(rand()%10), 0, (float)(rand()%10) }.normalize();
            enemy->set_velocity(velocity * enemy->max_velocity());
        } else {
            enemy->set_velocity_x(0);
            enemy->set_velocity_y(0);
        }
        enemy->set_clock((float)(rand()%3 + 3));
    }

    enemy->increase_velocity(0, -m_gravity*m_dt, 0);

    enemy_plataform_collision(enemy);

    if (enemy->velocity().y != 0)
        enemy->set_grounded(true);

    v3f displacement = enemy->velocity() * m_dt;

    enemy->translate(displacement);

    // look at player
    enemy->set_direction(m_player.center());
}

void Game::update(float dt) {
    m_dt = dt;
    handle_key_state();
    handle_player_movement();
    for (int i = 0; i < m_enemies.size(); i++) {
        m_enemies[i].increase_clock(-dt);
        handle_enemy_movement(&m_enemies[i]);
    }
}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_camera.mode() == objects::Camera::orbital) {
        gluLookAt(
            m_camera.position().x,
            m_camera.position().y,
            m_camera.position().z,

            m_player.center().x, 
            m_player.center().y, 
            m_player.center().z, 

            m_camera.normal().x,
            m_camera.normal().y,
            m_camera.normal().z
        );
    }
    else if (m_camera.mode() == objects::Camera::first_person) {
        m_camera.set_position(m_player.center() + (v3f){0, m_player.height()/2, 0});
        glRotatef(m_camera.angle_yz(),1,0,0);
        glRotatef(m_camera.angle_xz(),0,1,0);
        glTranslatef(-m_player.center().x, -m_camera.position().y, -m_camera.position().z);
    }

    for (auto enemy : m_enemies) {
        enemy.display();
    }

    m_arena.set_active_lights(m_active_lights);
    m_arena.set_show_axes(m_show_axes);
    m_arena.display();

    m_player.set_angle_xz(m_xz);
    m_player.display();
    m_player.set_show_axes(m_show_axes);
}