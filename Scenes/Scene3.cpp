#include "Scene3.h"
#include "util/pcgsolver.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <vector>
#include <cmath>
#include "src/util/pcgsolver.h"

/*

void Scene3::init() {
    gridWidth = 16;
    gridHeight = 16;
    domainSize = glm::vec2(1.0f, 1.0f);
    dx = domainSize.x / gridWidth;
    dy = domainSize.y / gridHeight;
    dt = 0.01f;
    diffusivity = 0.1f;

    // Initialize temperature grid with random noise
    temperature.assign(gridWidth * gridHeight, 0.0f);
    for (int i = 0; i < gridWidth * gridHeight; ++i) {
        temperature[i] = static_cast<float>(rand()) / RAND_MAX * 0.5f;
    }

    assembleMatrix();
}

void Scene3::simulateStep() {
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

void Scene3::onDraw(Renderer& renderer) {
    glm::vec3 boxScale(5.0f, 5.0f, 1.0f);
    renderer.drawWireCube(glm::vec3(0), boxScale);

    // Draw temperature field
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            float value = temperature[i + j * gridWidth];
            glm::vec4 color(value, 0.0f, 1.0f - value, 1.0f); // Gradient from blue to red
            glm::vec3 position = glm::vec3(
                -2.5f + i * dx * boxScale.x,
                -2.5f + j * dy * boxScale.y,
                0.0f
            );
            renderer.drawCube(position, glm::quat(), glm::vec3(dx, dy, 0.1f), color);
        }
    }
}

void Scene3::onGUI() {
    ImGui::SliderFloat("Time Step (dt)", &dt, 0.001f, 0.1f);
    ImGui::SliderFloat("Diffusivity", &diffusivity, 0.01f, 1.0f);
}

void Scene3::assembleMatrix() {
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
            float coef = diffusivity * dt / (dx * dx);
            matrix.set_element(idx, idx, 1.0f + 4.0f * coef);        // T[i, j]
            matrix.set_element(idx, idx - 1, -coef);                // T[i-1, j]
            matrix.set_element(idx, idx + 1, -coef);                // T[i+1, j]
            matrix.set_element(idx, idx - gridWidth, -coef);        // T[i, j-1]
            matrix.set_element(idx, idx + gridWidth, -coef);        // T[i, j+1]
        }
    }
}

std::vector<float> Scene3::assembleRHS() const {
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
*/