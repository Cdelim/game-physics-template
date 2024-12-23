#include "Scene1.h"
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

void Scene1::onDraw(Renderer& renderer) {

}

void Scene1::simulateStep() {
}

void Scene1::onGUI() {

}

void Scene1::init() {
    constexpr int nx = 6;  // Number of grid points in x
    constexpr int ny = 6;  // Number of grid points in y
    constexpr float dx = 1.0f;  // Grid spacing in x
    constexpr float dy = 1.0f;  // Grid spacing in y
    constexpr float alpha = 0.01f;  // Diffusivity
    constexpr float dt = 0.1f;  // Time step

    // Initial temperature field (6x6 grid)
    std::vector<std::vector<float>> T(nx, std::vector<float>(ny, 0.0f));

    // Example: Set initial condition
    T[1][3] = 100.0f;  // Heat source
    T[0][3] = 50.0f;
    T[0][5] = 75.0f;

    // Next temperature field
    std::vector<std::vector<float>> T_next = T;

    // Explicit Euler update for one time step
    for (int i = 1; i < nx - 1; ++i) {
        for (int j = 1; j < ny - 1; ++j) {
            float d2T_dx2 = (T[i + 1][j] - 2.0f * T[i][j] + T[i - 1][j]) / (dx * dx);
            float d2T_dy2 = (T[i][j + 1] - 2.0f * T[i][j] + T[i][j - 1]) / (dy * dy);

            T_next[i][j] = T[i][j] + dt * alpha * (d2T_dx2 + d2T_dy2);
        }
    }

    // Print the computed values
    std::cout << "T[1][3]: " << T_next[1][3] << std::endl;
    std::cout << "T[0][3]: " << T_next[0][3] << std::endl;
    std::cout << "T[0][5]: " << T_next[0][5] << std::endl;
}
