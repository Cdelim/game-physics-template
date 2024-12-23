#include "Scene2.h"
#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include <random>

/*
Authors:
David
Onurcan
Selin
Cem
*/

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
    // Convert the temperature grid to a 1D data vector
    std::vector<float> data;
    data.reserve(GRID_SIZE * GRID_SIZE);

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            // Assume temperature values are already normalized between 0 and 1
            data.push_back(temperatureGrid[i][j]);
        }
    }

    // Set up the colormap (e.g., use "viridis" or any other supported colormap)
    Colormap colormap("hot");

    // Define the screen position and size for rendering the heatmap
    glm::vec2 screenPosition(0.0f, 0.0f); // Bottom-left corner of the screen
    glm::vec2 screenSize(1.0f, 1.0f);

    // Draw the temperature heatmap using the 2D image renderer
    renderer.drawImage(data, GRID_SIZE, GRID_SIZE, colormap, screenPosition, screenSize);
}




void Scene2::onGUI() {
    ImGui::SliderFloat("Time Step Size", &dt, 0.001f, 0.1f);
    ImGui::SliderFloat("Thermal Diffusivity", &diffusivity, 0.01f, 1.0f);
}
