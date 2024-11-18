#include "Scene.h"


struct MassPoint {
    glm::vec3 position;
    glm::vec3  velocity;
    float mass;
};

struct Spring {
    MassPoint* pointA;
    MassPoint* pointB;
    float* restLength;
    float* stiffness;


    public:
    Spring(MassPoint* pointA,MassPoint* pointB, float* restLength, float* stiffness) : 
    pointA{pointA},pointB{pointB}, restLength{restLength},stiffness{stiffness}{}
};

class ComplexScene : public Scene
{
    public:
    float radius = 1.0f; // Adjust this as needed for the circle size
    float deltaTime = 0.1f;
    float stifness = 1;
    float restLength = 2.5f;
    float wallLength = 5;
    bool euler = true;
    bool midPoint = false;
    int timeStepCounter = 0;

    ComplexScene(); // Constructor to initialize mass points and springs
    void midPointIntegration(MassPoint &point,vec3 initialTotalForce);
    void eulerIntegration(MassPoint &point,vec3 totalForce);

    // Initializes the scene, setting up the mass-spring system and any relevant parameters
    void init() override;
    virtual void onDraw(Renderer &renderer) override;
    virtual void onGUI() override;
    // Updates the simulation using the selected integration method over time step `dt`
    void simulateStep() override;
    void CheckCollisions(MassPoint &point);
    std::vector<MassPoint> points;
    std::vector<Spring> springs;

    protected:
    std::vector<vec3> getPositions() const {
        std::vector<vec3> positions;
        for (const auto& point : points) {
            positions.push_back(point.position);
        }
        return positions;
    }
    void printState(const std::string& method) {
        std::cout << method << ":\n";
        auto& point = points[0];
        //for (const auto& point : points) {
            std::cout << "Position: (" << point.position.x << ", " << point.position.y << ","<<point.position.z <<"), "
                      << "Velocity: (" << point.velocity.x << ", " << point.velocity.y << ","<<point.velocity.z <<")\n";
        //}
        std::cout << "Stif,RestLength: (" << (*springs[0].stiffness) << ", " << (*springs[0].restLength) <<"), ";
    }

    vec3 computeSpringForce(const MassPoint& p1, const MassPoint& p2, const Spring& spring);

};