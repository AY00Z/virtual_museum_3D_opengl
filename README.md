# Musée virtuel OpenGL – Deux salles, deux ambiances

Projet de **Synthèse d'images** (Master 2, Université Gustave Eiffel – IGM, 2026/2027).

Application 3D temps réel en C++ / OpenGL 3.3 permettant de visiter, en vue à la première personne,
un musée composé de deux salles reliées par un court passage, chacune avec son propre modèle d'illumination.

Auteure : **Aya Boussaid**

---

## Compilation

Le projet utilise le framework des TD (glimac, GLFW, glad, glm), déjà inclus dans le dépôt.

### Sous Linux (machines de la fac)

Dépendances système (à installer si besoin) :

```bash
sudo apt install build-essential cmake libgl-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

Compilation, depuis la racine du dépôt :

```bash
mkdir build
cd build
cmake ..
make
```

L'exécutable est créé dans le dossier `bin/` à la racine du dépôt.

### Sous Windows (MinGW + VS Code)

1. Ouvrir le dossier du projet dans VS Code (extension **CMake Tools**).
2. `Ctrl + Shift + P` → **CMake: Configure**.
3. `F7` pour compiler.

---

## Lancement

Depuis la racine du dépôt :

```bash
./bin/TPProjet_musee          # Linux
.\bin\TPProjet_musee.exe      # Windows
```

---

## Commandes

| Touche (AZERTY) | Touche (QWERTY) | Action |
|---|---|---|
| **Z** / Flèche haut | W / Flèche haut | Avancer |
| **S** / Flèche bas | S / Flèche bas | Reculer |
| **Q** / Flèche gauche | A / Flèche gauche | Se décaler à gauche |
| **D** / Flèche droite | D / Flèche droite | Se décaler à droite |
| **Souris** | Souris | Regarder autour de soi |
| **F1** | F1 | Activer / désactiver le mode fil de fer |
| **Échap** | Échap | Quitter l'application |

Les touches de déplacement sont définies par leur **emplacement physique** sur le clavier :
elles fonctionnent donc aussi bien en AZERTY (ZQSD) qu'en QWERTY (WASD).

---

## Fonctionnalités

### Réalisées

- [x] Géométrie des deux salles (20 m × 24 m) et du passage (2 m × 4 m), conforme au plan du sujet, sans plafond
- [x] Caméra FPS : regard à la souris, déplacement fluide au clavier (indépendant du nombre d'images par seconde), hauteur des yeux fixe
- [x] Collisions avec les murs, avec glissement le long des parois
- [x] Mode fil de fer (F1)
- [x] Sortie propre de l'application (Échap), libération automatique des ressources OpenGL

### À venir

- [ ] Skybox visible depuis tout le musée
- [ ] Salle 1 : éclairage de Phong avec deux lumières (dont une mobile), objet texturé, objet animé
- [ ] Salle 2 : modèle d'illumination différent, surface transparente ou miroir
- [ ] Changement de modèle d'illumination au niveau du passage
- [ ] Deux œuvres minimum par salle
- [ ] Touches : pause de l'animation, allumage / extinction d'une lumière de la salle 1

---

## Organisation du code

Tout le code du projet se trouve dans le dossier `TPProjet/` :

| Fichier | Rôle |
|---|---|
| `main.cpp` | Création de la fenêtre, boucle principale, gestion du clavier et de la souris |
| `Vertex3D.hpp` | Structure d'un sommet 3D : position, normale, coordonnées de texture |
| `Mesh.hpp / .cpp` | Maillage : gère le VBO, l'IBO et le VAO, libérés automatiquement à la destruction (RAII) |
| `MuseumLayout.hpp / .cpp` | Plan du musée : dimensions, construction du sol et des murs, zone praticable (collisions) |
| `Camera.hpp / .cpp` | Caméra FPS : position, angles de vue, déplacement horizontal, matrice View |
| `shaders/` | Shaders GLSL (vertex et fragment) |

### Repère utilisé

- Axe **Y vers le haut**, sol en `y = 0`, **1 unité = 1 mètre**.
- Salle 1 : `x ∈ [-21, -1]`, salle 2 : `x ∈ [1, 21]`, les deux avec `z ∈ [-12, 12]`.
- Passage : `x ∈ [-1, 1]`, `z ∈ [-2, 2]`.
- La frontière entre les deux modèles d'illumination (pointillé vert du sujet) est en **`x = 0`**.
