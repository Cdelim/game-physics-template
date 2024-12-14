#pragma once

#include "Scene.h"

class Scene1 : public Scene
{
    public:
    virtual void init() override;
};



class RigidBodyBox {
    public:
    std::string name;

    glm::vec3 position;    
    glm::vec3 velocity;      
    glm::vec3 force;          

    glm::quat orientation;    
    glm::vec3 angularVelocity; 
    glm::vec3 angularMomentum;
    glm::vec3 torque;          

    float mass;              
    glm::mat3 inverseInertiaTensor; 

    glm::vec3 dimensions;     

    RigidBodyBox();

    RigidBodyBox(const glm::vec3& pos, const glm::vec3& dims, float mass, const glm::quat& initialOrientation);

    RigidBodyBox(const std::string &name, const glm::vec3& pos, const glm::vec3& dims, float mass, const glm::quat& initialOrientation);

    void clearForces();
    void applyForce(const glm::vec3& f, const glm::vec3& point);
    void integrate(float dt);
    void collide(RigidBodyBox &boxB, glm::vec3 point, glm::vec3 normal,  float restitution);
    
    void printState();
    void printWorldPositionAndVelocity(glm::vec3 bodySpacepoint);
};
