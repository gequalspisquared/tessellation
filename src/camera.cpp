#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    : m_position(position) 
    , m_up(up)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_world_up(up)
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_movement_speed(2.5f)
    , m_mouse_sensitivity(0.1f)
    , m_zoom(90.0f) 
{
    update_camera_vectors();    
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::process_keyboard(CameraMovement direction, float delta_time) {
    float velocity = m_movement_speed * delta_time;

    switch (direction) {
        case CameraMovement::FORWARD:
            m_position += m_front * velocity;
            break;
        case CameraMovement::BACKWARD:
            m_position -= m_front * velocity;
            break;
        case CameraMovement::LEFT:
            m_position -= m_right * velocity;
            break;
        case CameraMovement::RIGHT:
            m_position += m_right * velocity;
            break;
        default:
            break;
    }
}

void Camera::process_mouse(float xoffset, float yoffset, bool constrain_pitch) {
    xoffset *= m_mouse_sensitivity;
    yoffset *= m_mouse_sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrain_pitch) {
        if (m_pitch > 89.0f) {
            m_pitch = 89.0f;
        } else if (m_pitch < -89.0f) {
            m_pitch = -89.0f;
        }
    }

    update_camera_vectors();
}

void Camera::process_scroll(float yoffset) {
    m_zoom -= yoffset;

    if (m_zoom < 1.0f) {
        m_zoom = 1.0f;
    } else if (m_zoom > 100.0f) {
        m_zoom = 100.0f;
    }
}

float Camera::get_zoom() const {
    return m_zoom;
}

void Camera::update_camera_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}