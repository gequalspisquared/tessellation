#pragma once

#include <glm/glm.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0), 
           const glm::vec3& up = glm::vec3(0.0, 1.0, 0.0),
           float yaw = 0.0, float pitch = 0.0);
    
    glm::mat4 get_view_matrix() const;

    void process_keyboard(CameraMovement direction, float delta_time);
    void process_mouse(float xoffset, float yoffset, bool constrain_pitch = true);
    void process_scroll(float yoffset);

    float get_zoom() const;

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    float m_yaw;
    float m_pitch;
    float m_movement_speed;
    float m_mouse_sensitivity;
    float m_zoom;

    void update_camera_vectors();
};