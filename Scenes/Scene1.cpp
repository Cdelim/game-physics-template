#include "Scene1.h"

void Scene1::init(){
    glm::quat initialOrientation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    RigidBodyBox box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.6f, 0.4f), 2.0f, initialOrientation);

    box.applyForce(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.3f, 0.5f, 0.25f));
    box.integrate(2);
    box.clearForces();

    box.printState();
    box.printWorldPositionAndVelocity(glm::vec3(-0.3f, -0.5f, -0.25f));
}

RigidBodyBox::RigidBodyBox()
    : position(0.0f), velocity(0.0f), force(0.0f), 
      orientation(glm::quat(0.0f, 0.0f, 0.0f, 0.0f)), 
      angularVelocity(0.0f), angularMomentum(0.0f),
      torque(0.0f), mass(0.0f),
      inverseInertiaTensor(glm::mat3(0.0f)), 
      dimensions(0.0f, 0.0f, 0.0f) {
}

RigidBodyBox::RigidBodyBox(const glm::vec3& pos, const glm::vec3& dims, float mass, const glm::quat& initialOrientation)
    : name("box"), position(pos), velocity(0.0f), force(0.0f),
      orientation(initialOrientation),
      angularVelocity(0.0f), angularMomentum(0.0f), torque(0.0f), mass(mass),
      dimensions(dims) {

    float w2 = dims.x * dims.x;
    float h2 = dims.y * dims.y;
    float d2 = dims.z * dims.z;
    inverseInertiaTensor = glm::inverse(
        glm::mat3(
            (1.0f / 12.0f) * mass * (h2 + d2), 0, 0,
            0, (1.0f / 12.0f) * mass * (w2 + d2), 0,
            0, 0, (1.0f / 12.0f) * mass * (w2 + h2)
        )
    );
}

RigidBodyBox::RigidBodyBox(const std::string &name, const glm::vec3& pos, const glm::vec3& dims, float mass, const glm::quat& initialOrientation)
    : name(name), position(pos), velocity(0.0f), force(0.0f),
      orientation(initialOrientation),
      angularVelocity(0.0f), angularMomentum(0.0f), torque(0.0f), mass(mass),
      dimensions(dims) {

    float w2 = dims.x * dims.x;
    float h2 = dims.y * dims.y;
    float d2 = dims.z * dims.z;
    inverseInertiaTensor = glm::inverse(
        glm::mat3(
            (1.0f / 12.0f) * mass * (h2 + d2), 0, 0,
            0, (1.0f / 12.0f) * mass * (w2 + d2), 0,
            0, 0, (1.0f / 12.0f) * mass * (w2 + h2)
        )
    );
}

void RigidBodyBox::clearForces() {
    force = glm::vec3(0.0f);
    torque = glm::vec3(0.0f);
}

void RigidBodyBox::applyForce(const glm::vec3& f, const glm::vec3& point) {
    force += f;
    glm::vec3 bodySpacePoint = point - position; 
    torque += glm::cross(bodySpacePoint, f);
}

void RigidBodyBox::integrate(float dt) {
    // Euler Step 
    position += velocity * dt;
    velocity += dt * force / mass;

    glm::quat angularVelocityQuat(0.0f, angularVelocity.x, angularVelocity.y, angularVelocity.z);
    orientation += 0.5f * dt * angularVelocityQuat * orientation;
    orientation = glm::normalize(orientation); 

  
    angularMomentum += dt * torque;
    glm::mat3 worldSpaceInertiaTensor = glm::mat3_cast(orientation) * inverseInertiaTensor * glm::transpose(glm::mat3_cast(orientation));
    angularVelocity = worldSpaceInertiaTensor * angularMomentum;
}

void RigidBodyBox::printState(){
    std::cout << "Mass Point Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
    std::cout << "Linear Velocity: " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
    std::cout << "Angular Velocity: " << angularVelocity.x << ", " << angularVelocity.y << ", " << angularVelocity.z << std::endl;
    glm::vec3 euler = glm::eulerAngles(orientation);
    std::cout << "Orientation (Euler): " << glm::degrees(euler.x) << ", " << glm::degrees(euler.y) << ", " << glm::degrees(euler.z) << std::endl;
}

void RigidBodyBox::printWorldPositionAndVelocity(glm::vec3 bodySpacepoint){
    glm::vec3 worldSpacePosition = position + glm::mat3_cast(orientation) * bodySpacepoint;
    glm::vec3 worldSpaceVelocity = velocity + glm::cross(angularVelocity, bodySpacepoint);
    std::cout << "World Space Position: " << worldSpacePosition.x << ", " << worldSpacePosition.y << ", " << worldSpacePosition.z << std::endl;
    std::cout << "World Space Velocity: " << worldSpaceVelocity.x << ", " << worldSpaceVelocity.y << ", " << worldSpaceVelocity.z << std::endl;
}
