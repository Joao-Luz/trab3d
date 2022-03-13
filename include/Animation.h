#pragma once

#include <vector>

#include "VectorMath.h"

typedef struct {
    float time;
    v4f angles;
} t_keyframe;

class Animation {
private:
    std::vector<t_keyframe> m_keyframes;
    float m_duration, m_elapsed_time;
    v4f m_current_angles;
public:
    Animation();
    Animation(float duration);

    v4f current_angles();

    void add_keyframe(float time, v4f angles);
    v4f update(float dt);
    void stop();

    ~Animation() {}
};
