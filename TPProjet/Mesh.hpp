#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <vector>
#include "Vertex3D.hpp"

// Un maillage = un VBO (les sommets) + un IBO (les indices) + un VAO (la description)
// Il crée ses buffers à la construction et les libère à la destruction.
class Mesh
{
public:
    Mesh(const std::vector<Vertex3D> &vertices, const std::vector<uint32_t> &indices);
    ~Mesh();

    // Interdit la copie : deux copies libéreraient deux fois les mêmes buffers
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    void draw() const;

private:
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_vao;
    GLsizei m_indexCount;
};