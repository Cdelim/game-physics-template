#pragma once
#include "Scene.h"
#include <random>

class Scene1 : public Scene
{
    public:
        virtual void onGUI() override;
        virtual void onDraw(Renderer& renderer) override;
        virtual void simulateStep() override;
        virtual void init() override;
};