#include "Scene2.h"
#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include <random>

// Constants
constexpr int GRID_SIZE = 16;
constexpr float DEFAULT_DT = 0.01f;
constexpr float DEFAULT_DIFFUSIVITY = 0.1f;
std::vector<std::vector<float>> temperatureGrid; // Current temperature grid
std::vector<std::vector<float>> nextTemperatureGrid; // Buffer for next time step
float dt = DEFAULT_DT; // Time step size
float diffusivity = DEFAULT_DIFFUSIVITY; // Thermal diffusivity

void Scene2::initializeRandomTemperature() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (i == 0 || i == GRID_SIZE - 1 || j == 0 || j == GRID_SIZE - 1) {
                temperatureGrid[i][j] = 0.0f; // Boundary condition
            }
            else {
                temperatureGrid[i][j] = dis(gen); // Random noise
            }
        }
    }
}

void Scene2::applyHeatEquationStep() {
    for (int i = 1; i < GRID_SIZE - 1; ++i) {
        for (int j = 1; j < GRID_SIZE - 1; ++j) {
            float laplacian = temperatureGrid[i + 1][j] +
                temperatureGrid[i - 1][j] +
                temperatureGrid[i][j + 1] +
                temperatureGrid[i][j - 1] -
                4 * temperatureGrid[i][j];

            nextTemperatureGrid[i][j] = temperatureGrid[i][j] + dt * diffusivity * laplacian;
        }
    }

    // Swap grids
    std::swap(temperatureGrid, nextTemperatureGrid);
}

void Scene2::init() {
    temperatureGrid.resize(GRID_SIZE, std::vector<float>(GRID_SIZE, 0.0f));
    nextTemperatureGrid.resize(GRID_SIZE, std::vector<float>(GRID_SIZE, 0.0f));
    initializeRandomTemperature();
}

void Scene2::simulateStep() {
    applyHeatEquationStep();
}

void Scene2::onDraw(Renderer& renderer) {
    // Draw the wireframe box
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    // The size of each grid cell based on the cube size (5 units in both X and Y)
    float cellWidth = 5.0f / GRID_SIZE;  // Grid size: 16x16


    // Loop through the grid and draw each cell as a cube
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            // The temperature at the current grid cell
            float temperature = temperatureGrid[i][j];

            // Color based on the temperature (simple heatmap effect)
            glm::vec4 color = glm::vec4(temperature, 0.0f, 1.0f - temperature, 1.0f);

            // Calculate the position of each cell in the grid, scaling to fit the cube
            glm::vec3 position = glm::vec3(i * cellWidth - 2.5f, j * cellWidth - 2.5f, 0.0f); // Centered in the 5x5 box

            // The size of each cell (cellWidth by cellWidth in X and Y, unit size in Z)
            glm::vec3 size = glm::vec3(cellWidth, cellWidth, 1.0f);

            // Rotation is identity (no rotation for the grid cells)
            glm::quat rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            // Optionally, you can use flags for specific behaviors (e.g., no lighting)
            uint32_t flags = 0;

            // Draw each cube (grid cell) using instancing
            renderer.drawCube(position, rotation, size, color, flags);
        }
    }
}



void Scene2::onGUI() {
    ImGui::SliderFloat("Time Step Size", &dt, 0.001f, 0.1f);
    ImGui::SliderFloat("Thermal Diffusivity", &diffusivity, 0.01f, 1.0f);
}
