#pragma once

#include "Scene.hpp"

namespace YAML
{
class Node;
class Emitter;
} // namespace YAML

namespace Engine
{

class SceneSerializer
{
public:
    SceneSerializer() = default;
    ~SceneSerializer() = default;

    static void Serialize(const std::shared_ptr<Scene> &scene, const std::string &filePath);
    static bool Deserialize(const std::shared_ptr<Scene> &scene, const std::string &filePath);

private:
    static void SerializeEntity(YAML::Emitter &out, const Engine::Entity &entity);
    static Entity DeserializeEntity(const std::shared_ptr<Scene> &scene, const YAML::Node &entity);
};

} // namespace Engine