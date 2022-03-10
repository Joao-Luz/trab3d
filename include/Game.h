#pragma once

#include <string>
#include <unordered_map>

#include "Objects/Camera.h"
#include "Objects/Object.h"
#include "Objects/Player.h"
#include "Arena.h"

class Game {
private:
    int m_window_width, m_window_height;
    std::string m_window_name;

    std::unordered_map<std::string, int> m_textures;

    Arena m_arena;
    objects::Camera m_camera;
    objects::Player m_player;

    int m_last_x, m_last_y;
    float m_last_phi, m_last_theta;
    float m_phi, m_theta;
    float m_yz, m_xz;
    bool m_key_state[256];
    bool m_show_axes;
    bool m_active_lights[8];

    void load(std::string level_file_path);
    void load_arena(std::string path);
    void load_texture(std::string path, std::string name);
public:
    Game();
    ~Game() {}
    
    void init(int window_width, int window_height, std::string window_name, int* argc, char** argv);
    void run();
    int get_texture(std::string name);

    void display();
    void handle_mouse_click(int button, int sate, int x, int y);
    void handle_mouse_move(int x, int y);
    void handle_key_down(unsigned char key, int x, int y);
    void handle_key_up(unsigned char key, int x, int y);
    void update(float dt);
};

extern Game game;
