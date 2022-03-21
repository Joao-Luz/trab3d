#include "Objects/Player.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Game.h"
#include "Objects/Box.h"

namespace objects {

Player::Player(float x, float y, float z, float height, float radius, Game* game) : Character(x, y, z, height, radius, game) {
    m_flashlight_on = false;
    m_flashlight = LightSource(0, 0, 0, 7);
    m_flashlight.set_spotlight(true);
    m_flashlight.set_spot_direction((v3f){1, 0, 0});
    m_flashlight.set_active(false);
    m_won = false;

    m_animations = std::unordered_map<std::string, Animation*>();

    Animation* running = new Animation(0.7);
    running->add_keyframe(0   , {10, -10, 15, -90});
    running->add_keyframe(0.16, {-30, 0, 95, -90});
    running->add_keyframe(0.33 , {-35, -90, 30, 0});
    running->add_keyframe(0.49, {15, -90, 10, -10});
    running->add_keyframe(0.66, {95, -90, -30, 0});
    running->add_keyframe(0.85, {30, 0, -35, -90});

    m_animations.insert({"running", running});

    Animation* backwards = new Animation(0.7);
    backwards->add_keyframe(0   , {45, 0, -10, -95});
    backwards->add_keyframe(0.16, {30, 0, 0, -90});
    backwards->add_keyframe(0.33 , {15, -90, 10, -10});
    backwards->add_keyframe(0.49, {-10, -95, 45, 0});
    backwards->add_keyframe(0.66, {0, -90, 30, 0});
    backwards->add_keyframe(0.85, {10, -10, 15, -90});

    m_animations.insert({"backwards", backwards});

    Animation* jumping = new Animation(1.2);
    jumping->add_keyframe(0   , {45, -45, -10, -80});
    jumping->add_keyframe(0.33, {40, -40, 40, -40});
    jumping->add_keyframe(0.66 , {-10, -80, 45, -45});

    m_animations.insert({"jumping", jumping});

    Animation* still = new Animation(1);
    still->add_keyframe(0   , {0, 0, 0, 0});

    m_animations.insert({"still", still});

    m_current_animation = still;

    m_gun = Box({0, 0, 0}, {0.3, 0.11, 0.11});
    m_gun.set_origin(0.3, 0, 0);
    m_gun.set_color(0.1, 0.1, 0.1, 1);

    m_head = Box({0, 0, 0}, {0.2, 0.2, 0.2});
    m_head.set_origin(0.5, 0, 0.5);
    m_head.set_texture(m_game->get_texture("head"));

    m_pony_tail = Box({0, 0, 0}, {0.07, 0.2, 0.07});
    m_pony_tail.set_origin(0.5, 1, 0.5);
    m_pony_tail.set_color(0.1333, 0.1255, 0.2039, 1);

    m_body = Box({0, 0, 0}, {0.1, 0.4, 0.25});
    m_body.set_origin(0.5, 0.5, 0.5);
    m_body.set_texture(m_game->get_texture("body"));

    m_upper_right_arm = Box({0, 0, 0}, {0.1, 0.2, 0.1});
    m_upper_right_arm.set_origin(0.5, 1.0, 0);
    m_upper_right_arm.set_texture(m_game->get_texture("arms"));

    m_lower_right_arm = Box({0, 0, 0}, {0.2, 0.1, 0.1});
    m_lower_right_arm.set_origin(0, 0, 0);
    m_lower_right_arm.set_texture(m_game->get_texture("arms"));

    m_upper_left_arm = Box({0, 0, 0}, {0.2, 0.1, 0.1});
    m_upper_left_arm.set_origin(0, 1, 1);
    m_upper_left_arm.set_texture(m_game->get_texture("arms"));

    m_lower_left_arm = Box({0, 0, 0}, {0.2, 0.1, 0.1});
    m_lower_left_arm.set_origin(0, 1, 1);
    m_lower_left_arm.set_texture(m_game->get_texture("arms"));
    
    m_upper_right_leg = Box({0, 0, 0}, {0.1, 0.2, 0.1});
    m_upper_right_leg.set_origin(0.5, 1, 0.5);
    m_upper_right_leg.set_texture(m_game->get_texture("upper_leg"));

    m_lower_right_leg = Box({0, 0, 0}, {0.1, 0.2, 0.1});
    m_lower_right_leg.set_origin(0.5, 1, 0.5);
    m_lower_right_leg.set_texture(m_game->get_texture("lower_leg"));

    m_upper_left_leg = Box({0, 0, 0}, {0.1, 0.2, 0.1});
    m_upper_left_leg.set_origin(0.5, 1, 0.5);
    m_upper_left_leg.set_texture(m_game->get_texture("upper_leg"));

    m_lower_left_leg = Box({0, 0, 0}, {0.1, 0.2, 0.1});
    m_lower_left_leg.set_origin(0.5, 1, 0.5);
    m_lower_left_leg.set_texture(m_game->get_texture("lower_leg"));



}

void Player::set_current_animation(std::string name) {
    auto new_animation = m_animations[name];
    if (m_current_animation != nullptr && m_current_animation != new_animation) {
        m_current_animation->stop();
    }
    
    m_current_animation = new_animation;
}

void Player::plataform_collision(Box plataform, float dt) {
    v3f next_position = center() + m_velocity*dt;

    v3f center = plataform.center();
    float width = plataform.scale().x;
    float height = plataform.scale().y;
    if ((next_position.x + m_radius) > (center.x - width/2) && 
        (next_position.x - m_radius) < (center.x + width/2) &&
        (next_position.y + m_height/2) > (center.y - height/2) &&
        (next_position.y - m_height/2) < (center.y + height/2)) {
        
        // front
        if ((this->center().x + m_radius) < (plataform.center().x - width/2)) {
            this->set_velocity_x(0);
        } else 

        // back
        if ((this->center().x - m_radius) > (plataform.center().x + width/2)) {
            this->set_velocity_x(0);
        } else

        // top
        if ((this->center().y + m_height/2) < (plataform.center().y - height/2)) {
            this->set_velocity_y(0);
        } else {

        // bottom
        // ((this->center().y) > (plataform.center().y + height/2 + m_height/2)) {
            this->set_velocity_y(0);
            this->set_grounded(true);
            this->set_center_y(plataform.center().y + height/2 + m_height/2);
        }
    }
}

void Player::enemy_collision(Enemy* enemy, float dt) {
    v3f next_player_position = this->center() + m_velocity*dt;
    v3f next_enemy_position = enemy->center() + enemy->velocity()*dt;

    if ((next_player_position.y + m_height/2) > (next_enemy_position.y - enemy->height()/2) &&
        (next_player_position.y - m_height/2) < (next_enemy_position.y + enemy->height()/2)) {
        
        next_player_position.y = 0;
        next_enemy_position.y = 0;
        float distance = (next_enemy_position - next_player_position).norm();

        if (distance < (m_radius + enemy->radius())) {
            // bottom top
            if ((this->center().y - m_height/2) > (enemy->center().y + enemy->height()/2)) {
                this->set_velocity_y(0);
                this->set_grounded(true);
            // sides
            } else {
                float a = m_velocity.x * enemy->velocity().x;
                a = a > 0 ? 1 : -1;
                float b = m_velocity.z * enemy->velocity().z;
                b = b > 0 ? 1 : -1;
                enemy->set_velocity_x(a*enemy->velocity().x);
                enemy->set_velocity_z(b*enemy->velocity().z);

                this->set_velocity_x(0);
                this->set_velocity_z(0);
            }
        }
    }
}

void Player::update_current_animation(float dt) { 
    m_current_animation->update(dt);
}

void Player::display() {

    glPushMatrix();

        v3f center = this->center();

        glTranslatef(center.x, center.y, center.z);

        v3f rotation_vector = (v3f){1, 0, 0}.cross(m_direction);
        float angle = 180*acos(m_direction.normalize().dot({1, 0, 0}))/M_PI;

        glRotatef(angle, rotation_vector.x, rotation_vector.y, rotation_vector.z);

        glScalef(m_height, m_height, m_height);
        // body
        glPushMatrix();
            glTranslatef(0, 0.1, 0);
            m_body.display();
        glPopMatrix();

        
        glPushMatrix();
            glTranslatef(0, 0.3, 0.125);

            // rotate arm
            float cos = m_direction.dot(m_aim);
            if (cos > 1) cos = 1.0f;
            if (cos < -1) cos = -1.0f;
            angle = 180*acos(cos)/M_PI;
            if (m_aim.y <= 0) angle = -angle;

            glRotatef(angle, 0, 0, 1);
            m_upper_right_arm.display();
            glTranslatef(0, -0.2, 0);
            m_lower_right_arm.display();

            glTranslatef(0.2, 0.1, 0);
            m_gun.display();

            if (m_flashlight_on) {
                glTranslatef(0.21, 0, 0);
                m_flashlight.set_position(0, 0, 0);
                m_flashlight.set_direction(1, 0, 0);
                m_flashlight.set_active(true);
            } else {
                m_flashlight.set_active(false);
            }
            m_flashlight.display();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, 0.3, -0.125);
            // rotate arm
            glRotatef(angle, 0, 0, 1);

            glRotatef(-20, 0, 0, 1);
            glRotatef(-20, 0, 1, 0);
            m_upper_left_arm.display();
            glTranslatef(0.2, 0, 0);
            glRotatef(-60, 0, 1, 0);
            m_lower_left_arm.display();

        glPopMatrix();

        // head
        glPushMatrix();
            glTranslatef(0, 0.3, 0);
            glRotatef(angle, 0, 0, 1);
            m_head.display();
            glTranslatef(-0.1, 0.15, 0);
            glRotatef(-30, 0, 0, 1);
            m_pony_tail.display();
        glPopMatrix();

        v4f animation_angles = m_current_animation->current_angles();

        // right leg
        glPushMatrix();
            glTranslatef(0, -0.1, 0.075);
            glRotatef(animation_angles.x, 0, 0, 1);
            m_upper_right_leg.display();
            glTranslatef(0, -0.2, 0);
            glRotatef(animation_angles.y, 0, 0, 1);
            m_lower_right_leg.display();
        glPopMatrix();

        // left leg
        glPushMatrix();
            glTranslatef(0, -0.1, -0.075);
            glRotatef(animation_angles.z, 0, 0, 1);
            m_upper_left_leg.display();
            glTranslatef(0, -0.2, 0);
            glRotatef(animation_angles.w, 0, 0, 1);
            m_lower_left_leg.display();
        glPopMatrix();

    glPopMatrix();
}

void Player::arena_collision(Arena arena, float dt) {
    v3f next_position = this->center() + m_velocity*dt;

    // left
    if ((next_position.z - m_radius) < 0) {
        set_velocity_z(0);
        set_center_z(m_radius);
    }

    // right
    if ((next_position.z + m_radius) > arena.length()) {
        set_velocity_z(0);
        set_center_z(arena.length() - m_radius);
    }

    // back
    if ((next_position.x - m_radius) < 0) {
        set_velocity_x(0);
        set_center_x(m_radius);
    }

    // front
    if ((next_position.x + m_radius) > arena.width()) {
        set_velocity_x(0);
        set_center_x(arena.width() - m_radius);
        m_won = true;
        m_alive = false;
    }

    // bottom
    if ((next_position.y - m_height/2) < 0) {
        set_velocity_y(0);
        set_center_y(m_height/2);
        set_grounded(true);
    }

    // top
    if ((next_position.y + m_height/2) > arena.height()) {
        set_velocity_y(0);
        set_center_y(arena.height() - m_height/2);
    }

}

v3f Player::gun_position() {
    v3f right = {-m_direction.z, -m_direction.y, m_direction.x};

    return this->center() + (v3f(0, 0.25, 0) + (right*0.175) + (m_aim*0.2))*m_height;
}

}
