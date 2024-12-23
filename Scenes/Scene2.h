#pragma once
#include "Scene.h"
#include <vector>
#include <glm/glm.hpp>

class Scene2 : public Scene {
public:
    virtual void onGUI() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    void applyHeatEquationStep();
    void initializeRandomTemperature();
};
