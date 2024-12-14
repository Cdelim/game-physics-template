#include "Scene2.h"
#include <imgui.h>
#include <string>

void Scene2::init(){
    glm::quat initialOrientation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    box = RigidBodyBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.6f, 0.4f), 2.0f, initialOrientation);
    box.applyForce(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.3f, 0.5f, 0.25f));
}

void Scene2::onDraw(Renderer& renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    renderer.drawCube(box.position, box.orientation, box.dimensions, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);
}

void Scene2::simulateStep(){
    float realtimeDt = ImGui::GetIO().DeltaTime;

    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)){   
        auto pos = ImGui::GetMousePos();
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto x = pos.x * right;
            auto y = pos.y * up;
            auto dx = -drag.x * right;
            auto dy = drag.y * up;
            box.applyForce(glm::normalize(dx+dy), glm::normalize(x+y));
        }
    }
    box.integrate(dt);
    box.clearForces();

}

void Scene2::onGUI(){
    ImGui::SliderFloat("Time Step Size", &dt, 0, 1);

    std::string force = "Force: (" + std::to_string(box.force.x) + ", " 
                        + std::to_string(box.force.y) + ", " 
                        + std::to_string(box.force.z) + ")";

    std::string torque = "Torque: (" + std::to_string(box.torque.x) + ", " 
                        + std::to_string(box.torque.y) + ", " 
                        + std::to_string(box.torque.z) + ")";

    ImGui::Text("%s", force.c_str());
    ImGui::Text("%s", torque.c_str());
}