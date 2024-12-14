#include "Scene.h"
#include <map>

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"


using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"1 Single Step", creator<Scene1>()},
    {"2 Simulation", creator<Scene2>()},
    {"3 Collision", creator<Scene3>()},
    {"4 Complex Scene", creator<Scene4>()},
};
