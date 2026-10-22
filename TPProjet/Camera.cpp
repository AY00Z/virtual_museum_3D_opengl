#include "Camera.hpp"

Camera::Camera(const glm::vec3 &position, float yaw, float pitch)
    : m_position(position), m_yaw(yaw), m_pitch(pitch)
{
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    m_yaw += deltaYaw;
    m_pitch += deltaPitch;

    // Empêche de se retourner la tête en regardant trop haut ou trop bas
    if (m_pitch > 89.f)
        m_pitch = 89.f;
    if (m_pitch < -89.f)
        m_pitch = -89.f;
}

void Camera::move(float forward, float right)
{
    float yawRad = glm::radians(m_yaw);

    // Direction "devant" projetée au sol (composante Y = 0) : on ne s'envole pas
    glm::vec3 forwardDir(glm::cos(yawRad), 0.f, glm::sin(yawRad));
    // Direction "à droite" : perpendiculaire à "devant", toujours horizontale
    glm::vec3 rightDir(-glm::sin(yawRad), 0.f, glm::cos(yawRad));

    m_position += forward * forwardDir + right * rightDir;
}

glm::vec3 Camera::getFrontVector() const
{
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);

    // Coordonnées sphériques -> vecteur direction (de longueur 1)
    return glm::vec3(glm::cos(pitchRad) * glm::cos(yawRad),
                     glm::sin(pitchRad),
                     glm::cos(pitchRad) * glm::sin(yawRad));
}

glm::mat4 Camera::getViewMatrix() const
{
    // On regarde depuis m_position vers le point situé juste devant soi
    return glm::lookAt(m_position, m_position + getFrontVector(), glm::vec3(0.f, 1.f, 0.f));
}