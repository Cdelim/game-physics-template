#include "Scene3.h"
#include <imgui.h>
#include <string>
#include "util/CollisionDetection.h"

void Scene3::init(){
    glm::quat box1Orientation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    RigidBodyBox box1("box1", glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.6f, 0.4f), 2.0f, box1Orientation);
    box1.applyForce(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    glm::quat box2Orientation1 = glm::angleAxis(glm::radians(110.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::quat box2Orientation2 = glm::angleAxis(glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    RigidBodyBox box2("box2", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 2.0f, box2Orientation1 * box2Orientation2);
    box2.applyForce(glm::vec3(0.0f, -50.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    boxes.push_back(box1);
    boxes.push_back(box2);
}

void Scene3::onDraw(Renderer& renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    for (auto& box : boxes){
        renderer.drawCube(box.position, box.orientation, box.dimensions, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);
}

void Scene3::simulateStep(){
    for (auto& boxA : boxes){
        // collision check
        for (auto& boxB : boxes){
            if(boxB.name != boxA.name){
                glm::mat4 matA = getWorldFromObj(boxA);
                glm::mat4 matB = getWorldFromObj(boxB);
                CollisionInfo info = collisionTools::checkCollisionSAT(matA, matB);
                if(info.isColliding){
                    std::cout << "Collision found!" << std::endl;
                    boxA.collide(boxB, info.collisionPointWorld, info.normalWorld, c);
                }
            }
            
        }

        // integration
        boxA.integrate(dt);
        boxA.clearForces();
    }
    

}

void Scene3::onGUI(){
    ImGui::SliderFloat("Time Step Size", &dt, 0, 1);
    ImGui::SliderFloat("Coefficient of Restitution", &c, 0, 1);
}

glm::mat4 Scene3::getWorldFromObj(const RigidBodyBox &box){
    glm::mat4 rotationMatrix = glm::toMat4(box.orientation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), box.dimensions);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), box.position);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

void RigidBodyBox::collide(RigidBodyBox &boxB, glm::vec3 point, glm::vec3 normal, float restitution){
    // need body space position and velocity
    glm::vec3 posBodySpaceA = point - position;
    glm::vec3 posBodySpaceB = point - boxB.position;
    glm::vec3 velocityA = velocity + glm::cross(angularVelocity, posBodySpaceA);
    glm::vec3 velocityB = boxB.velocity + glm::cross(boxB.angularVelocity, posBodySpaceB);

    glm::vec3 relativeVelocity = velocityA - velocityB;
    float relativeVelocityAlongNormal = glm::dot(relativeVelocity, normal);

    if(relativeVelocityAlongNormal < 0){
        glm::mat3 rotatedInverseInertiaTensorA = glm::mat3_cast(orientation) * inverseInertiaTensor * glm::transpose(glm::mat3_cast(orientation));
        glm::mat3 rotatedInverseInertiaTensorB = glm::mat3_cast(boxB.orientation) * boxB.inverseInertiaTensor * glm::transpose(glm::mat3_cast(boxB.orientation));

        float impulse = (-1.0f * (1.0f + restitution)) * glm::dot(relativeVelocity, normal);
        impulse /=  1.0f/mass + 
                    1.0f/boxB.mass + 
                    glm::dot((rotatedInverseInertiaTensorA * glm::cross(glm::cross(posBodySpaceA, normal), posBodySpaceA)), normal) +
                    glm::dot((rotatedInverseInertiaTensorB * glm::cross(glm::cross(posBodySpaceB, normal), posBodySpaceB)), normal);

        // apply impulse
        velocity += (impulse * normal) / mass;
        boxB.velocity -= (impulse * normal) / boxB.mass;
        angularMomentum += glm::cross(posBodySpaceA, impulse * normal);
        boxB.angularMomentum -= glm::cross(posBodySpaceB, impulse * normal);
    }    
}