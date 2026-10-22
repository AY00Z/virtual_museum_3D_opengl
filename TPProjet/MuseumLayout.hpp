#pragma once

#include <cstdint>
#include <vector>
#include "Vertex3D.hpp"

// Plan du musée (en mètres). Repère : Y vers le haut, sol en y = 0.
// La frontière entre les deux salles (pointillé vert du sujet) est en x = 0.
namespace MuseumLayout
{
    const float WALL_HEIGHT = 5.f;

    // Salle 1 (gauche)
    const float ROOM1_XMIN = -21.f;
    const float ROOM1_XMAX = -1.f;

    // Salle 2 (droite)
    const float ROOM2_XMIN = 1.f;
    const float ROOM2_XMAX = 21.f;

    // Les deux salles ont la même profondeur
    const float ROOM_ZMIN = -12.f;
    const float ROOM_ZMAX = 12.f;

    // Passage entre les deux salles (x de -1 à 1)
    const float PASSAGE_ZMIN = -2.f;
    const float PASSAGE_ZMAX = 2.f;

    // Distance minimale entre le visiteur et un mur (en mètres)
    const float PLAYER_RADIUS = 0.3f;

    // Renvoie true si le visiteur peut se tenir au point (x, z) du sol
    bool isWalkable(float x, float z);

    // Remplit "vertices" et "indices" avec le sol et tous les murs du musée
    void buildGeometry(std::vector<Vertex3D> &vertices, std::vector<uint32_t> &indices);
}