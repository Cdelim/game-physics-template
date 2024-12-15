#pragma once

#include "Scene1.h"

class Scene4 : public Scene
{
    std::vector<RigidBodyBox> boxes;
    float dt = 0.01f;
    float c = 0.5f;

    virtual void onDraw(Renderer &renderer) override;

    virtual void simulateStep() override;

    virtual void onGUI() override;

    virtual void init() override;

    glm::mat4 getWorldFromObj(const RigidBodyBox &box);
};