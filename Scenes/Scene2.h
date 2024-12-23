#pragma once
#include "Scene.h"
#include <vector>
#include <glm/glm.hpp>

/*
Authors:
David
Onurcan
Selin
Cem
*/

class Scene2 : public Scene {
public:
    virtual void onGUI() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual void applyHeatEquationStep();
    virtual void initializeRandomTemperature();
};
