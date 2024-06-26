#include "Scene.h"
#include <memory>
#include <map>
#include <functional>

#include "Scene1.h"
#include "Scene2.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

std::map<std::string, SceneCreator> scenesCreators = {
    {"Scene1", []()
     { return std::make_unique<Scene1>(); }},
    {"Scene2", []()
     { return std::make_unique<Scene2>(); }},
    // add more Scene types here
};