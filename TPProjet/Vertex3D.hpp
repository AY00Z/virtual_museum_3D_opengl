#pragma once

#include <glimac/glm.hpp>

// Un sommet 3D : position, normale (pour l'éclairage) et coordonnées de texture
struct Vertex3D
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex3D() {}
    Vertex3D(const glm::vec3 &pos, const glm::vec3 &norm, const glm::vec2 &uv)
        : position(pos), normal(norm), texCoords(uv) {}
};