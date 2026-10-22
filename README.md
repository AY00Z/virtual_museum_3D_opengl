# 🏛️ Virtual Museum — Real-Time 3D Engine in C++ / OpenGL

A real-time 3D application written from scratch in **modern C++** and **OpenGL 3.3 (core profile, shader-based)**.
Explore a two-room virtual museum in first-person view, each room with its own lighting model and visual identity.

> 🚧 **Work in progress** — the engine foundations are done; lighting, textures and artworks are under active development.

---

## ✨ Highlights

- **Custom rendering pipeline** built directly on OpenGL: VBO / IBO / VAO management, GLSL shaders, MVP matrices, depth testing
- **First-person camera** with mouse look, smooth frame-rate–independent movement (delta time) and fixed eye height
- **Collision system** that keeps the visitor inside the museum, with natural sliding along walls
- **Procedural level geometry** generated from a single, data-driven floor plan
- **Clean resource management** using RAII: every GPU buffer is released automatically, no memory leaks
- **Cross-platform** build with CMake (Linux / GCC and Windows / MinGW)

---

## 🛠️ Tech Stack

| | |
|---|---|
| **Language** | C++17 |
| **Graphics API** | OpenGL 3.3 core, GLSL 330 |
| **Windowing & input** | GLFW |
| **OpenGL loader** | glad |
| **Math** | GLM |
| **Build system** | CMake |

---

## 🎮 Controls

| Key | Action |
|---|---|
| **W A S D** (QWERTY) / **Z Q S D** (AZERTY) / **Arrow keys** | Move |
| **Mouse** | Look around |
| **F1** | Toggle wireframe mode |
| **Esc** | Quit |

Movement keys are mapped by **physical position**, so they work out of the box on both QWERTY and AZERTY keyboards.

---

## 🚀 Build & Run

### Linux

```bash
# Dependencies (Debian / Ubuntu)
sudo apt install build-essential cmake libgl-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Build
mkdir build && cd build
cmake ..
make

# Run (from the repository root)
cd ..
./bin/TPProjet_musee
```

### Windows (MinGW + VS Code)

1. Open the folder in VS Code with the **CMake Tools** extension.
2. `Ctrl + Shift + P` → **CMake: Configure**, then `F7` to build.
3. Run `.\bin\TPProjet_musee.exe`.

All third-party libraries (GLFW, glad, GLM) are bundled in the repository — no extra installation needed.

---

## 🧱 Architecture

```
TPProjet/
├── main.cpp            # Window, main loop, input handling
├── Vertex3D.hpp        # Vertex layout: position, normal, UV
├── Mesh.hpp/.cpp       # GPU mesh (VBO + IBO + VAO), RAII-managed
├── MuseumLayout.hpp/.cpp  # Floor plan, geometry generation, walkable-area collisions
├── Camera.hpp/.cpp     # First-person camera (yaw / pitch), view matrix
└── shaders/            # GLSL vertex & fragment shaders
```

**Design choices**

- **`Mesh`** owns its OpenGL buffers and is non-copyable, which prevents double-free bugs and guarantees cleanup.
- **`MuseumLayout`** is the single source of truth for the museum dimensions: walls, floors and collisions are all derived from the same constants.
- **Collisions** test the X and Z axes separately, so the player slides along walls instead of stopping dead.

**World coordinates:** Y-up, 1 unit = 1 meter. Each room is 20 m × 24 m, joined by a 2 m × 4 m corridor.

---

## 🗺️ Roadmap

- [x] 3D museum geometry (two rooms + corridor, open sky)
- [x] First-person camera with smooth movement
- [x] Wall collisions with sliding
- [x] Wireframe debug mode
- [ ] Skybox
- [ ] Room 1 — Phong lighting with multiple lights (including a moving light), textured and animated artworks
- [ ] Room 2 — distinct stylized lighting model, mirror / transparent surfaces
- [ ] Seamless lighting-model switch when walking between rooms
- [ ] 3D model loading (`.obj`) for the artworks

---

## 👩‍💻 Author

**Aya Boussaid** — [@AY00Z](https://github.com/AY00Z)

Feel free to reach out or open an issue!
