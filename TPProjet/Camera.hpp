#pragma once

#include <glimac/glm.hpp>

// Caméra à la première personne (FPS) : une position + deux angles
//  - yaw   : rotation gauche / droite (autour de l'axe Y), en degrés
//  - pitch : regarder en haut / en bas, en degrés, limité à [-89, 89]
class Camera
{
public:
    Camera(const glm::vec3 &position, float yaw, float pitch);

    // Tourne la tête (valeurs en degrés)
    void rotate(float deltaYaw, float deltaPitch);

    // Se déplace sur le plan horizontal : la hauteur des yeux ne change jamais
    //  forward > 0 : avancer, < 0 : reculer
    //  right   > 0 : aller à droite, < 0 : aller à gauche
    void move(float forward, float right);

    // Matrice View à envoyer au shader
    glm::mat4 getViewMatrix() const;

    glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3 &position) { m_position = position; }

private:
    // Direction du regard calculée à partir des deux angles
    glm::vec3 getFrontVector() const;

    glm::vec3 m_position;
    float m_yaw;
    float m_pitch;
};