#include "Animation.h"

#include <iostream>

Animation::Animation() : Animation(1) { }

Animation::Animation(float duration) : m_elapsed_time{0}, m_duration{duration} {
    m_elapsed_time = 0;
    m_duration = duration;
    m_current_angles = {0, 0, 0, 0};
    m_keyframes = std::vector<t_keyframe>();
}

v4f Animation::update(float dt) {

    m_elapsed_time += dt;
    if (m_elapsed_time > m_duration) m_elapsed_time = (m_elapsed_time-m_duration);
    float normalized_elapsed_time = m_elapsed_time/m_duration;

    auto current_keyframe = m_keyframes[0];
    auto next_keyframe = m_keyframes[0];
    for (auto keyframe : m_keyframes) {
        if (keyframe.time <= normalized_elapsed_time)
            current_keyframe = keyframe;
        else {
            next_keyframe = keyframe;
            break;
        }
    }
    if (current_keyframe.time == m_keyframes.back().time && m_keyframes.size() > 1) {
        next_keyframe = m_keyframes[0];
    }

    auto angles = current_keyframe.angles;
    auto next_angles = next_keyframe.angles;

    float a = normalized_elapsed_time - current_keyframe.time;
    float b = next_keyframe.time - current_keyframe.time;
    if (b < 0) b += 1;
    float proportion = b == 0 ? 0 : a/b;

    v4f interpolated = angles + (next_angles - angles)*proportion;
    m_current_angles = interpolated;

    return interpolated;
}

void Animation::stop() {
    m_elapsed_time = 0;
}
v4f Animation::current_angles() { return m_current_angles; }
void Animation::add_keyframe(float time, v4f angles) {
    t_keyframe keyframe = {time, angles};
    m_keyframes.push_back(keyframe);
}
