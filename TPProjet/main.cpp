#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <vector>
#include "Mesh.hpp"
#include "MuseumLayout.hpp"
#include "Camera.hpp"

using namespace glimac;

int window_width = 800;
int window_height = 800;

bool wireframe = false; // mode fil de fer activé ou non

static void key_callback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    // F1 : bascule entre affichage plein et fil de fer
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        wireframe = !wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }
}

static void mouse_button_callback(GLFWwindow * /*window*/, int /*button*/, int /*action*/, int /*mods*/) {}
static void scroll_callback(GLFWwindow * /*window*/, double /*xoffset*/, double /*yoffset*/) {}
static void cursor_position_callback(GLFWwindow * /*window*/, double /*xpos*/, double /*ypos*/) {}

static void size_callback(GLFWwindow * /*window*/, int width, int height)
{
    window_width = width;
    window_height = height;
}

int main(int /*argc*/, char **argv)
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Musee virtuel", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    /* Bloc { } : les objets OpenGL (Program, Mesh) sont détruits ici,
       AVANT glfwTerminate(), tant que le contexte OpenGL existe encore */
    {
        /*********************************
         * INITIALIZATION CODE
         *********************************/
        FilePath applicationPath(argv[0]);
        Program program = loadProgram(applicationPath.dirPath() + "TPProjet/shaders/basic3D.vs.glsl",
                                      applicationPath.dirPath() + "TPProjet/shaders/basic3D.fs.glsl");
        program.use();

        // Récupère l'emplacement de l'uniform dans le programme (une seule fois)
        GLint uMVPMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVPMatrix");

        // Construit le sol et les murs du musée dans un seul Mesh
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
        MuseumLayout::buildGeometry(vertices, indices);
        Mesh museum(vertices, indices);

        // Caméra : au centre de la salle 1, yeux à 1.70 m, regard vers le passage (yaw = 0 -> axe +X)
        Camera camera(glm::vec3(-11.f, 1.7f, 0.f), 0.f, 0.f);

        // Réglages des contrôles
        const float MOVE_SPEED = 4.f;          // mètres par seconde
        const float MOUSE_SENSITIVITY = 0.1f;  // degrés par pixel de souris

        // Capture la souris : curseur invisible et bloqué dans la fenêtre
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Position précédente de la souris et heure de la frame précédente
        double lastMouseX, lastMouseY;
        glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        double lastTime = glfwGetTime();

        // Active le test de profondeur : les surfaces proches cachent les lointaines
        glEnable(GL_DEPTH_TEST);

        /*********************************
         * APPLICATION LOOP
         *********************************/
        while (!glfwWindowShouldClose(window))
        {
            /* --- deltaTime : temps écoulé depuis la frame précédente (en secondes) --- */
            double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - lastTime);
            lastTime = currentTime;

            /* --- Souris : tourner la tête --- */
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            float mouseDeltaX = static_cast<float>(mouseX - lastMouseX);
            float mouseDeltaY = static_cast<float>(mouseY - lastMouseY);
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            // Souris vers le haut = Y écran qui diminue = regarder vers le haut, d'où le signe -
            camera.rotate(mouseDeltaX * MOUSE_SENSITIVITY, -mouseDeltaY * MOUSE_SENSITIVITY);

            /* --- Clavier : se déplacer (touches testées à chaque frame -> mouvement fluide) --- */
            // GLFW_KEY_W/A/S/D désignent l'EMPLACEMENT physique des touches :
            // sur un clavier AZERTY, ce sont les touches Z/Q/S/D
            float forward = 0.f;
            float right = 0.f;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                forward += 1.f;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                forward -= 1.f;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                right += 1.f;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                right -= 1.f;

            /* --- Collisions : on n'accepte le déplacement que s'il reste dans le musée --- */
            glm::vec3 oldPosition = camera.getPosition();
            camera.move(forward * MOVE_SPEED * deltaTime, right * MOVE_SPEED * deltaTime);
            glm::vec3 wantedPosition = camera.getPosition();

            // On teste X puis Z séparément : contre un mur, on glisse le long au lieu d'être bloqué net
            glm::vec3 newPosition = oldPosition;
            if (MuseumLayout::isWalkable(wantedPosition.x, newPosition.z))
                newPosition.x = wantedPosition.x;
            if (MuseumLayout::isWalkable(newPosition.x, wantedPosition.z))
                newPosition.z = wantedPosition.z;
            camera.setPosition(newPosition);

            // Taille réelle de la zone de dessin (peut changer si on redimensionne la fenêtre)
            int fbWidth, fbHeight;
            glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
            glViewport(0, 0, fbWidth, fbHeight);

            glClearColor(0.55f, 0.75f, 0.95f, 1.f); // bleu ciel provisoire
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /* --- Matrices --- */
            // Model : le musée est déjà construit à sa place dans le monde -> matrice identité
            glm::mat4 model = glm::mat4(1.f);

            // View : donnée par la caméra FPS
            glm::mat4 view = camera.getViewMatrix();

            // Projection : perspective, angle de vue 60°, objets visibles de 0.1 m à 200 m
            float aspect = (fbHeight > 0) ? static_cast<float>(fbWidth) / fbHeight : 1.f;
            glm::mat4 projection = glm::perspective(glm::radians(60.f), aspect, 0.1f, 200.f);

            glm::mat4 mvp = projection * view * model;
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));

            museum.draw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}