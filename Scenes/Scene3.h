#pragma once
#include "Scene.h"
#include <glm/glm.hpp>

class Scene3 : public Scene {
public:
    std::vector<float> temperature;
    std::vector<float> matrix;

/*
    virtual void onGUI() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual std::vector<float> assembleRHS() const;
    virtual void assembleMatrix();*/
};
