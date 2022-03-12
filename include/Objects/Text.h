#pragma once

#include <string>

#include "VectorMath.h"
#include "./Object.h"

namespace objects {

class Text : public Object {
private:
    std::string m_text;
    float m_screen_width, m_screen_height;
public:
    Text(v3f position, std::string text = "");
    Text(float x, float y, float z, std::string text = "");

    Text() {}
    ~Text() {}

    std::string text() { return m_text; }
    void set_text(std::string text) { m_text = text; }

    float screen_width() { return m_screen_width; }
    void set_screen_width(float screen_width) { m_screen_width = screen_width; }

    float screen_height() { return m_screen_height; }
    void set_screen_height(float screen_height) { m_screen_height = screen_height; }

    void set_screen(float width, float height) { m_screen_width = width; m_screen_height = height; }

    void display();
};
   
}
