#include "MuseumLayout.hpp"

namespace MuseumLayout
{
    // Ajoute un quad (4 sommets a, b, c, d dans l'ordre du contour) aux tableaux.
    // width et height servent aux coordonnées de texture : 1 unité de texture = 1 mètre,
    // ce qui permettra plus tard de répéter une texture sur un grand mur.
    static void addQuad(std::vector<Vertex3D> &vertices, std::vector<uint32_t> &indices,
                        const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, const glm::vec3 &d,
                        const glm::vec3 &normal, float width, float height)
    {
        uint32_t first = static_cast<uint32_t>(vertices.size()); // indice du sommet "a"

        vertices.push_back(Vertex3D(a, normal, glm::vec2(0.f, 0.f)));
        vertices.push_back(Vertex3D(b, normal, glm::vec2(width, 0.f)));
        vertices.push_back(Vertex3D(c, normal, glm::vec2(width, height)));
        vertices.push_back(Vertex3D(d, normal, glm::vec2(0.f, height)));

        // Deux triangles, comme dans l'exo 6 : (0, 1, 2) et (0, 2, 3)
        indices.push_back(first + 0);
        indices.push_back(first + 1);
        indices.push_back(first + 2);
        indices.push_back(first + 0);
        indices.push_back(first + 2);
        indices.push_back(first + 3);
    }

    // Sol horizontal (y = 0) sur le rectangle [xmin, xmax] x [zmin, zmax], normale vers le haut
    static void addFloor(std::vector<Vertex3D> &vertices, std::vector<uint32_t> &indices,
                         float xmin, float xmax, float zmin, float zmax)
    {
        addQuad(vertices, indices,
                glm::vec3(xmin, 0.f, zmax), glm::vec3(xmax, 0.f, zmax),
                glm::vec3(xmax, 0.f, zmin), glm::vec3(xmin, 0.f, zmin),
                glm::vec3(0.f, 1.f, 0.f), xmax - xmin, zmax - zmin);
    }

    // Mur vertical allant du point (x0, z0) au point (x1, z1) au sol, de hauteur WALL_HEIGHT
    static void addWall(std::vector<Vertex3D> &vertices, std::vector<uint32_t> &indices,
                        float x0, float z0, float x1, float z1, const glm::vec3 &normal)
    {
        float length = glm::length(glm::vec2(x1 - x0, z1 - z0));
        addQuad(vertices, indices,
                glm::vec3(x0, 0.f, z0), glm::vec3(x1, 0.f, z1),
                glm::vec3(x1, WALL_HEIGHT, z1), glm::vec3(x0, WALL_HEIGHT, z0),
                normal, length, WALL_HEIGHT);
    }

    void buildGeometry(std::vector<Vertex3D> &vertices, std::vector<uint32_t> &indices)
    {
        // Normales des murs : toujours tournées vers l'intérieur (là où se trouve le visiteur)
        const glm::vec3 towardPlusX(1.f, 0.f, 0.f);
        const glm::vec3 towardMinusX(-1.f, 0.f, 0.f);
        const glm::vec3 towardPlusZ(0.f, 0.f, 1.f);
        const glm::vec3 towardMinusZ(0.f, 0.f, -1.f);

        /* --- Sols --- */
        addFloor(vertices, indices, ROOM1_XMIN, ROOM1_XMAX, ROOM_ZMIN, ROOM_ZMAX);
        addFloor(vertices, indices, ROOM1_XMAX, ROOM2_XMIN, PASSAGE_ZMIN, PASSAGE_ZMAX); // passage
        addFloor(vertices, indices, ROOM2_XMIN, ROOM2_XMAX, ROOM_ZMIN, ROOM_ZMAX);

        /* --- Salle 1 --- */
        addWall(vertices, indices, ROOM1_XMIN, ROOM_ZMIN, ROOM1_XMIN, ROOM_ZMAX, towardPlusX);  // mur du fond (ouest)
        addWall(vertices, indices, ROOM1_XMIN, ROOM_ZMIN, ROOM1_XMAX, ROOM_ZMIN, towardPlusZ);  // mur nord
        addWall(vertices, indices, ROOM1_XMIN, ROOM_ZMAX, ROOM1_XMAX, ROOM_ZMAX, towardMinusZ); // mur sud
        // Mur est, coupé en deux pour laisser l'ouverture du passage
        addWall(vertices, indices, ROOM1_XMAX, ROOM_ZMIN, ROOM1_XMAX, PASSAGE_ZMIN, towardMinusX);
        addWall(vertices, indices, ROOM1_XMAX, PASSAGE_ZMAX, ROOM1_XMAX, ROOM_ZMAX, towardMinusX);

        /* --- Passage --- */
        addWall(vertices, indices, ROOM1_XMAX, PASSAGE_ZMIN, ROOM2_XMIN, PASSAGE_ZMIN, towardPlusZ);
        addWall(vertices, indices, ROOM1_XMAX, PASSAGE_ZMAX, ROOM2_XMIN, PASSAGE_ZMAX, towardMinusZ);

        /* --- Salle 2 --- */
        addWall(vertices, indices, ROOM2_XMAX, ROOM_ZMIN, ROOM2_XMAX, ROOM_ZMAX, towardMinusX); // mur du fond (est)
        addWall(vertices, indices, ROOM2_XMIN, ROOM_ZMIN, ROOM2_XMAX, ROOM_ZMIN, towardPlusZ);  // mur nord
        addWall(vertices, indices, ROOM2_XMIN, ROOM_ZMAX, ROOM2_XMAX, ROOM_ZMAX, towardMinusZ); // mur sud
        // Mur ouest, coupé en deux pour laisser l'ouverture du passage
        addWall(vertices, indices, ROOM2_XMIN, ROOM_ZMIN, ROOM2_XMIN, PASSAGE_ZMIN, towardPlusX);
        addWall(vertices, indices, ROOM2_XMIN, PASSAGE_ZMAX, ROOM2_XMIN, ROOM_ZMAX, towardPlusX);
    }

    // true si (x, z) est dans le rectangle [xmin, xmax] x [zmin, zmax]
    static bool isInside(float x, float z, float xmin, float xmax, float zmin, float zmax)
    {
        return x >= xmin && x <= xmax && z >= zmin && z <= zmax;
    }

    bool isWalkable(float x, float z)
    {
        const float r = PLAYER_RADIUS;

        // Salle 1, rétrécie de r de chaque côté pour ne pas coller aux murs
        if (isInside(x, z, ROOM1_XMIN + r, ROOM1_XMAX - r, ROOM_ZMIN + r, ROOM_ZMAX - r))
            return true;

        // Salle 2, même principe
        if (isInside(x, z, ROOM2_XMIN + r, ROOM2_XMAX - r, ROOM_ZMIN + r, ROOM_ZMAX - r))
            return true;

        // Passage : plus étroit de r de chaque côté en z, mais prolongé de r en x
        // pour se raccorder aux zones des deux salles (sinon il resterait un "trou" à l'entrée)
        if (isInside(x, z, ROOM1_XMAX - r, ROOM2_XMIN + r, PASSAGE_ZMIN + r, PASSAGE_ZMAX - r))
            return true;

        return false;
    }
}