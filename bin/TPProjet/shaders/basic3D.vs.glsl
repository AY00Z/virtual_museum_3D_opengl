#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

// Matrice Projection * View * Model, envoyée par le C++ (uniform)
uniform mat4 uMVPMatrix;

out vec3 vNormal;
out vec2 vTexCoords;

void main() {
    vNormal = aVertexNormal;
    vTexCoords = aVertexTexCoords;
    gl_Position = uMVPMatrix * vec4(aVertexPosition, 1);
}