#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <filesystem>
#include <cmath>

#include <tinyxml2.h>

#include "Arena.h"
#include "Box.h"
#include "Camera.h"
#include "Plane.h"
#include "Player.h"
#include "VectorMath.h"

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b

Camera camera;
Player player;
Arena arena;
bool show_axes;
bool active_light[] = {true, true, true, true, true, true, true, true};
int window_width, window_height;

void load_arena_from_file(const char* path) {
    tinyxml2::XMLDocument level;
    level.LoadFile(path);

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

    player = Player(x-base_x, arena_height - ( y - base_y) - height/2, z, height);
    arena = Arena(arena_width, arena_height);

    // camera
    camera = Camera(0, 2, 5);
    camera.set_direction(camera.position());
    camera.set_up(0, 1, 0);

    // plataforms
    for (auto rect = level.FirstChild()->FirstChildElement("rect"); rect != nullptr; rect = rect->NextSiblingElement("rect")) {
        if (std::string(rect->Attribute("fill")) == "blue") continue;
        float x = std::stod(rect->Attribute("x")) - base_x;
        float y = arena_height - (std::stod(rect->Attribute("y")) - base_y);
        float width = std::stod(rect->Attribute("width"));
        float height = std::stod(rect->Attribute("height"));
        arena.add_plataform(x, y-height, width, height);
    }

    // enemies
    // for (auto circ = level.FirstChild()->FirstChildElement("circle"); circ != nullptr; circ = circ->NextSiblingElement("circle")) {
    //     if (std::string(circ->Attribute("fill")) == "green") continue;
    //     double x = std::stod(circ->Attribute("cx")) - base_x;
    //     double y = camera_width_height - (std::stod(circ->Attribute("cy")) - base_y);
    //     double height = 2*std::stod(circ->Attribute("r"));
    //     double width = height/5;
    //     auto enemy = this->add_enemy(x, y, width, height);
    //     enemy->set_velocity_x(m_player_speed);
    // }
}

void init(int window_width, int window_height, const char* path) {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glViewport (0, 0, (GLsizei)window_width, (GLsizei)window_height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50, (GLfloat)window_width/window_height, 1, 500);

    load_arena_from_file(path);
}


void display(void) {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (camera.mode() == Camera::orbital) {
        gluLookAt(
            camera.position().x,
            camera.position().y,
            camera.position().z,

            player.center().x, 
            player.center().y, 
            player.center().z, 

            camera.up().x,
            camera.up().y,
            camera.up().z
        );
    }
    else if (camera.mode() == Camera::first_person) {
        glRotatef(camera.angle_yz(),1,0,0);
        glRotatef(camera.angle_xz(),0,1,0);
        glTranslatef(-player.center().x, -player.center().y-1, -player.center().z);
    }

    arena.display();

    Box box(0, 0, 0);
    box.set_show_axes(show_axes);
    box.set_color(0, 1, 0);
    box.display();

    player.display();
    player.set_show_axes(show_axes);

    glutSwapBuffers();
}

int last_x, last_y = 0;
float last_phi, last_theta = 0;
float phi, theta = 0;
float yz, xz = 0;

void mouse_click(int button, int state, int x, int y) {

    if (button == 0 && state == 0) {
        last_x = x;
        last_y = y;
    }

    else if (button == 0 && state == 1) {
        last_phi = phi;
        last_theta = theta;
    }

    glutPostRedisplay();
}

float clamp(float val, float low, float high) {
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

void mouse_move(int x, int y) {
    float dphi = -2*M_PI*((float)(x - last_x)/window_width);
    float dtheta = M_PI*((float)(y - last_y)/window_height);

    phi = last_phi + dphi;
    theta = last_theta + dtheta;
    theta = clamp(theta, -M_PI/3, M_PI/3);

    yz = 180*(theta)/M_PI;
    xz = -180*(phi)/M_PI;

    float rho = 2*player.height();

    if (camera.mode() == Camera::orbital) {

        camera.set_position(
            rho*sin(phi)*cos(theta) + player.center().x,
            rho*sin(theta) + player.center().y,
            rho*cos(theta)*cos(phi) + player.center().z
        );

        camera.set_direction(player.center() - camera.position());
    }

    else if (camera.mode() == Camera::first_person) {
        camera.set_angle(yz, xz);

        camera.set_direction(player.center() - camera.position());
        camera.set_direction(
            -sin(phi)*cos(theta),
            -sin(theta),
            -cos(theta)*cos(phi)
        );
    }

    glutPostRedisplay();
}

int key_state[256] = { 0 };

void key_down(unsigned char key, int x, int y) {
    key_state[key] = 1;

    if (key == 'x') {
        Camera::camera_mode new_mode = (Camera::camera_mode)((camera.mode() + 1)%2);
        camera.set_mode(new_mode);

        if (new_mode == Camera::first_person) {
            camera.set_angle(yz, xz);
            v3f new_direction = {camera.direction().x, 0, -camera.direction().z};
            camera.set_direction(new_direction);
        } else {
            camera.set_position(
                5*sin(phi)*cos(theta) + player.position().x,
                5*sin(theta) + player.position().y,
                5*cos(theta)*cos(phi) + player.position().z
            );
        }
    }

    if (key == 'p') {
        show_axes = !show_axes;
    }
    if (key >= '0' && key <= '9') {
        int light_id = key-'0'-1;
        active_light[light_id] = !active_light[light_id]; 
    }
    arena.set_active_lights(active_light);

    glutPostRedisplay();
}

void key_up(unsigned char key, int x, int y) {
    key_state[key] = 0;
    glutPostRedisplay();
}

void idle() {
    v3f front  = camera.direction().normalize();
    front.y = 0;
    if (key_state['w']) {
        player.translate(front);
        camera.translate(front);
    }

    if (key_state['s']) {
        v3f back = {-front.x, -front.y, -front.z};
        player.translate(back);
        camera.translate(back);
    }

    if (key_state['d']) {
        v3f right = {-front.z, -front.y, front.x};
        player.translate(right);
        camera.translate(right);
    }

    if (key_state['a']) {
        v3f left = {front.z, front.y, -front.x};
        player.translate(left);
        camera.translate(left);
    }

    glutPostRedisplay();
}



int main(int argc, char** argv) {
    if (argc < 2 || !std::filesystem::exists(argv[1])) {
        std::cout << "Please, provide a valid level file directory\n";
        return 0;
    }

    window_width = window_height = 1000;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_width);
    glutCreateWindow("Trabalho 3D");
    init(window_width, window_height, argv[1]);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMotionFunc(mouse_move);
    glutMouseFunc(mouse_click);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glutMainLoop();
}