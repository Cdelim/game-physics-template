#include "Scene.h"
#include <random>


struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float lifetime;
};

class Scene1 : public Scene
{
    std::vector<Particle> particles;

    float pitch = 0.f;
    float roll = 0.f;
    float yaw = 0.f;

    float pitch_increment = 0.001f;
    float roll_increment = 0.002f;
    float yaw_increment = 0.003f;
    virtual void onGUI() override;

    int32_t launch_delay = 8;
    int32_t lastLaunch = 0;
    void launchSphere();

    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    struct inputState{
        bool space = false;
        bool w = false, a = false, s = false, d = false;
        bool e = false, q = false;
    };
    inputState keyState;

    public:
    Scene1() : gen(rd()), dis(0.f, 1.f) {}
};