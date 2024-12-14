#pragma once

#include "Scene1.h"

class Scene2 : public Scene
{
    RigidBodyBox box;
    float dt = 0.01;

    virtual void onDraw(Renderer &renderer) override;

    virtual void simulateStep() override;

    virtual void onGUI() override;

    virtual void init() override;
};