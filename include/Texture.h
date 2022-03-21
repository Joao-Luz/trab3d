#pragma once

#include <memory>
#include <unordered_map>
#include <string>

class Texture {
private:
    std::string m_name, m_path;
    int m_id, m_width, m_height;
    bool m_box;
    static Texture load_texture(std::string path, std::string name, bool box=false);
public:
    Texture(std::string name, std::string path, bool box=false);

    Texture() {}
    ~Texture() {}

    std::string name() { return m_name; }
    std::string path() { return m_path; }
    int id() { return m_id; }
    int width() { return m_width; }
    int height() { return m_height; }
    bool box() { return m_box; }
};