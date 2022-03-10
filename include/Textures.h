#pragma once

#include <memory>
#include <unordered_map>
#include <string>

class Textures : public std::unordered_map<std::string, int>{
private:
public:

    Textures() : std::unordered_map<std::string, int>() { }
    ~Textures() {}

    static int load_texture(std::string path, std::string name);
};