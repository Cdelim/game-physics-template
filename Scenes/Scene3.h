#pragma once
#include "Scene.h"
#include <vector>
#include <glm/glm.hpp>
#include "util/pcgsolver.h"

/*
Authors:
David
Onurcan
Selin
Cem
*/

class Scene3 : public Scene {
public:
    std::vector<float> temperature;
    SparseMatrix<float> matrix;
    int gridWidth = 16;
    int gridHeight = 16;
    glm::vec2 domainSize;
    float dx;
    float dy;
    float dt;
    float diffusivity;


    virtual void onGUI() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual std::vector<float> assembleRHS() const;
    virtual void assembleMatrix();
};
