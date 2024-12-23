#include "Scene.h"
#include <map>

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Demo Scene", creator<Scene>()},
    {"Scene1", creator<Scene1>()},
    {"Scene2", creator<Scene2>()},
    {"Scene3", creator<Scene3>()},
    
    // add more Scene types here
};
