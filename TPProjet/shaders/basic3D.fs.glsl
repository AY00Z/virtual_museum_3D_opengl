#version 330

in vec3 vNormal;
in vec2 vTexCoords;

out vec3 fFragColor;

void main() {
    // Affichage de débogage : chaque orientation de surface a sa propre couleur
    // (la normale, entre -1 et 1, est ramenée entre 0 et 1)
    fFragColor = normalize(vNormal) * 0.5 + 0.5;
}