#include "Scene4.h"
#include "util/pcgsolver.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <vector>
#include <cmath>
#include "src/util/pcgsolver.h"

/*
Authors:
David
Onurcan
Selin
Cem
*/

void Scene4::init() {
    gridWidth = 16;
    gridHeight = 16;
    domainSize = glm::vec2(1.0f, 1.0f);
    dx = domainSize[0] / gridWidth;
    dy = domainSize[1] / gridHeight;
    dt = 0.001f;
    diffusivity = 0.01f;

    // Initialize temperature grid with random noise
    temperature.assign(gridWidth * gridHeight, 0.0f);
    for (int i = 0; i < gridWidth * gridHeight; ++i) {
        temperature[i] = static_cast<float>(rand()) / RAND_MAX * 0.5f;
    }

    assembleMatrix();
}

void Scene4::simulateStep() {
    // Assemble the RHS (T^n) for the current temperature field
    std::vector<float> rhs = assembleRHS();

    // Prepare a vector to hold the result of the system (T^{n+1})
    std::vector<float> result(gridWidth * gridHeight, 0.0f);

    // Prepare variables for solver output
    float relative_residual = 0.0f;
    int iterations = 0;
    int precondition = 2; // Default precondition value, as suggested by the function prototype

    // Solve the linear system A * result = rhs using the provided SparsePCGSolver
    SparsePCGSolver<float> solver;
    bool success = solver.solve(matrix, rhs, result, relative_residual, iterations, precondition);

    // Check if the solver succeeded
    if (!success) {
        throw std::runtime_error("SparsePCGSolver failed to converge.");
    }

    // Update the temperature field with the solved values
    temperature = result;
}

void Scene4::onDraw(Renderer& renderer) {
    // Define the colormap to use for visualization
    Colormap colormap("hot"); // Change to your desired colormap, e.g., "hot", "plasma"

    // Screen position and size for the 2D heat map
    glm::vec2 screenPosition(0.0f, 0.0f); // Bottom-left corner of the screen
    glm::vec2 screenSize(1.0f, 1.0f);     // Size of the heatmap (normalized device coordinates)

    // Ensure temperature data is normalized to [0, 1] for colormap application
    float minTemp = *std::min_element(temperature.begin(), temperature.end());
    float maxTemp = *std::max_element(temperature.begin(), temperature.end());
    float tempRange = (maxTemp - minTemp) > 0.0f ? (maxTemp - minTemp) : 1.0f;

    // Normalize the temperature values and prepare the data for rendering
    std::vector<float> normalizedTemperature(temperature.size());
    for (size_t idx = 0; idx < temperature.size(); ++idx) {
        normalizedTemperature[idx] = (temperature[idx] - minTemp) / tempRange;
    }

    // Use the Renderer to draw the heat map
    renderer.drawImage(normalizedTemperature, gridHeight, gridWidth, colormap, screenPosition, screenSize);
}


void Scene4::onGUI() {
    auto first = ImGui::SliderFloat("Time Step (dt)", &dt, 0.001f, 0.1f);
    auto second = ImGui::SliderFloat("Diffusivity", &diffusivity, 0.01f, 1.0f);
    ImGui::SliderInt("GridWidth", &gridWidth, 4, 32);
    ImGui::SliderInt("GridHeight", &gridHeight, 4, 32);

    float domainSizeX = domainSize[0];
    float domainSizeY = domainSize[1];
    ImGui::SliderFloat("DomainSizeX", &domainSizeX, 1.0f, 4.0f);
    ImGui::SliderFloat("DomainSizeY", &domainSizeX, 1.0f, 4.0f);
   
    domainSize = glm::vec2(domainSizeX,domainSizeY); 
    dx = domainSize[0] / gridWidth;
    dy = domainSize[1] / gridHeight;

    

    assembleMatrix();


    
}

void Scene4::assembleMatrix() {
    int n = gridWidth * gridHeight;
    matrix.resize(n);
    matrix.zero();

    for (int j = 0; j < gridHeight; ++j) {
        for (int i = 0; i < gridWidth; ++i) {
            int idx = i + j * gridWidth;

            // Boundary conditions
            if (i == 0 || j == 0 || i == gridWidth - 1 || j == gridHeight - 1) {
                matrix.set_element(idx, idx, 1.0f); // Dirichlet BC: T = 0
                continue;
            }

            // Interior points
            float coef = (diffusivity * dt *ImGui::GetIO().DeltaTime)/ (dx * dx);
            matrix.set_element(idx, idx, 1.0f + 4.0f * coef);        // T[i, j]
            matrix.set_element(idx, idx - 1, -coef);                // T[i-1, j]
            matrix.set_element(idx, idx + 1, -coef);                // T[i+1, j]
            matrix.set_element(idx, idx - gridWidth, -coef);        // T[i, j-1]
            matrix.set_element(idx, idx + gridWidth, -coef);        // T[i, j+1]
        }
    }
}

std::vector<float> Scene4::assembleRHS() const {
    int n = gridWidth * gridHeight;
    std::vector<float> rhs(n, 0.0f);

    for (int j = 0; j < gridHeight; ++j) {
        for (int i = 0; i < gridWidth; ++i) {
            int idx = i + j * gridWidth;

            // Boundary conditions
            if (i == 0 || j == 0 || i == gridWidth - 1 || j == gridHeight - 1) {
                rhs[idx] = 0.0f; // Dirichlet BC: T = 0
                continue;
            }

            // Interior points
            rhs[idx] = temperature[idx];
        }
    }
    return rhs;
}
