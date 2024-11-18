#include "ComplexScene.h"
#include <imgui.h>

ComplexScene::ComplexScene() {
     int numPoints = 100;
     
     glm::vec3 center(2.5f, 2.5f, 2.5f); 

//Create soft body sphere
for (int i = 0; i < numPoints; ++i) {
    float phi = acos(1 - 2 * (i + 0.5f) / numPoints); 
    float theta = M_PI * (1 + sqrt(5)) * i; 

        glm::vec3 position(
            radius * sin(phi) * cos(theta),
            radius * sin(phi) * sin(theta),
            radius * cos(phi)
        );
   
    points.push_back(MassPoint{position+(wallLength*.5f), glm::vec3(0.0f), 10.0f});
    
    
}
// Connect All Points
for (int i = 0; i < numPoints; ++i) {
        for (int j = i + 1; j < numPoints; ++j) {
                springs.push_back(Spring{&points[i], &points[j], &restLength, &stifness});
        }
}
springs.push_back(Spring{&points[points.size() - 1], &points[0], &restLength, &stifness});

}

void ComplexScene::init() {
    ComplexScene();

     
}

void ComplexScene::onGUI(){
    ImGui::SliderFloat("Time Step", &deltaTime, 0, 1);
    ImGui::SliderFloat("Stifness", &stifness, 0, 50);
    ImGui::SliderFloat("Rest Length", &restLength, 1, 4);
    if (ImGui::Checkbox("Euler", &euler)) {
        if (euler) {
            midPoint = false;
        }
    }

    if (ImGui::Checkbox("Midpoint", &midPoint)) {
        if (midPoint) {
            euler = false;
        }
    }
    
}

void ComplexScene::onDraw(Renderer& renderer) {
    
    for (int i = 0; i < points.size(); ++i) {
        renderer.drawSphere(points.at(i).position, .1f, glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));
    }
    for (int i = 0; i < springs.size(); ++i) {
        auto& spring = springs.at(i);
        renderer.drawLine(spring.pointA->position, spring.pointB->position, glm::vec3(0.5f, 0.5f, 0.5f));
    }

    
    // Collision Area
    renderer.drawWireCube(vec3(wallLength*.5f),vec3(wallLength),vec3(1));
    
    //Global Axis Draw
    renderer.drawLine(vec3(0),vec3(0,1,0),vec3(1,0,0));
    renderer.drawLine(vec3(0),vec3(1,0,0),vec3(0,1,0));
    renderer.drawLine(vec3(0),vec3(0,0,1),vec3(0,0,1));
    

}

void ComplexScene::simulateStep() {
    
   for (auto& point : points) {
    glm::vec3 totalForce(0.0f);

    // Force Calculation
   for (const auto& spring : springs) {
        if (spring.pointA == &point || spring.pointB == &point) {
        MassPoint* otherPoint = (spring.pointA == &point) ? spring.pointB : spring.pointA;
            totalForce += computeSpringForce(point, *otherPoint, spring);
        }
    }


    // Gravity
    totalForce += glm::vec3(0.0f, -9.81f * point.mass, 0.0f);


    if(euler){
        eulerIntegration(point,totalForce);
    }
    else{
        midPointIntegration(point,totalForce);
    }
   

    // Check Collision in the WireBox
    CheckCollisions(point);
   }

}
void ComplexScene::CheckCollisions(MassPoint& point)
{
    if (point.position.y < 0 || point.position.y > 5)
    {
        point.velocity.y = -point.velocity.y * 0.5f; // Energy loss
        point.position.y = glm::clamp(point.position.y, 0.0f, 5.0f);
    }
    if (point.position.x < 0 || point.position.x > 5)
    {
        point.velocity.x = -point.velocity.x * 0.5f; // Energy loss
        point.position.x = glm::clamp(point.position.x, 0.0f, 5.0f);
    }
    if (point.position.z < 0 || point.position.z > 5)
    {
        point.velocity.z = -point.velocity.z * 0.5f; // Energy loss
        point.position.z = glm::clamp(point.position.z, 0.0f, 5.0f);
    }
}

glm::vec3 ComplexScene::computeSpringForce(const MassPoint& p1, const MassPoint& p2, const Spring& spring) {
    glm::vec3 direction = p2.position - p1.position;
    float currentLength = glm::length(direction);

    //Divided by 0 check
    if (currentLength != 0) {
        direction /= currentLength;
    }

    // force calculation
    float forceMagnitude = -(*spring.stiffness) * (currentLength - (*spring.restLength));


    return (direction * forceMagnitude);
}

void ComplexScene::eulerIntegration(MassPoint& point,vec3 totalForce){

    float realtimeDt = ImGui::GetIO().DeltaTime;
     glm::vec3 acceleration = totalForce / point.mass;
    point.position += point.velocity * deltaTime * realtimeDt;
    point.velocity += acceleration * deltaTime * realtimeDt;
}

void ComplexScene::midPointIntegration(MassPoint& point, glm::vec3 initialTotalForce) {
    float realtimeDt = ImGui::GetIO().DeltaTime;
    float halfDt = 0.5f * deltaTime * realtimeDt;

    // Initial acc
    glm::vec3 initialAcceleration = initialTotalForce / point.mass;

    // MidPoint Calculation
    MassPoint midPoint;
    midPoint.mass = 10;
    midPoint.velocity = point.velocity + initialAcceleration * halfDt;
    midPoint.position = point.position + point.velocity * halfDt;

    // Midpoint Force Calculation
    glm::vec3 midpointTotalForce(0.0f);
    for (const auto& spring : springs) {
        if (spring.pointA == &point || spring.pointB == &point) {
            MassPoint* otherPoint = (spring.pointA == &point) ? spring.pointB : spring.pointA;
            midpointTotalForce += computeSpringForce(midPoint, *otherPoint, spring);
        }
    }
    //Gravity
    midpointTotalForce += glm::vec3(0.0f, -9.81f * point.mass, 0.0f);

    // MidPoint acc
    glm::vec3 midpointAcceleration = midpointTotalForce / midPoint.mass;

    // Update Position
    point.position +=  midPoint.velocity * deltaTime * realtimeDt;
    point.velocity += midpointAcceleration * deltaTime * realtimeDt;
}