#include "Scene4.h"
#include <imgui.h>
#include <string>
#include "util/CollisionDetection.h"

void Scene4::init(){
    glm::quat initialOrientation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    RigidBodyBox box1("box1", glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 2.0f, initialOrientation);
    RigidBodyBox box2("box2", glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 2.0f, initialOrientation);
    RigidBodyBox box3("box3", glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 2.0f, initialOrientation);
    RigidBodyBox box4("box4", glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 2.0f, initialOrientation);

    box1.applyForce(glm::vec3(-1.1f, 0.0f, -1.1f), glm::vec3(1.1f, 0.0f, 1.1f));
    box2.applyForce(glm::vec3(-1.1f, 0.0f, 1.1f), glm::vec3(1.1f, 0.0f, -1.1f));
    box3.applyForce(glm::vec3(1.1f, 0.0f, -1.1f), glm::vec3(-1.1f, 0.0f, 1.1f));
    box4.applyForce(glm::vec3(1.1f, 0.0f, 1.1f), glm::vec3(-1.1f, 0.0f, -1.1f));

    boxes.push_back(box1);
    boxes.push_back(box2);
    boxes.push_back(box3);
    boxes.push_back(box4);
}

void Scene4::onDraw(Renderer& renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    for (auto& box : boxes){
        renderer.drawCube(box.position, box.orientation, box.dimensions, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);
}

void Scene4::simulateStep(){
    if(ImGui::IsKeyDown(ImGuiKey_Space)){   
        for (auto& box : boxes){
            auto force = - box.position;
            box.applyForce(glm::normalize(force), glm::vec3(0.0f));
        }
    }

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

void Scene4::onGUI(){
    ImGui::SliderFloat("Time Step Size", &dt, 0, 1);
    ImGui::SliderFloat("Coefficient of Restitution", &c, 0, 1);
    ImGui::Text("Press (and hold) Space to pull Cubes to the center.");
}

glm::mat4 Scene4::getWorldFromObj(const RigidBodyBox &box){
    glm::mat4 rotationMatrix = glm::toMat4(box.orientation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), box.dimensions);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), box.position);
    return translationMatrix * rotationMatrix * scaleMatrix;
}
