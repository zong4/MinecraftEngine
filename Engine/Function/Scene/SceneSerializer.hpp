#pragma once

#include "Scene.hpp"

namespace YAML
{
class Node;
class Emitter;
} // namespace YAML

namespace MCEngine
{

class SceneSerializer
{
public:
    SceneSerializer() = default;
    ~SceneSerializer() = default;

    static void Serialize(const std::shared_ptr<Scene> &scene, const std::string &filePath);
    static bool Deserialize(const std::shared_ptr<Scene> &scene, const std::string &filePath);

private:
    static void SerializeEntity(YAML::Emitter &out, const MCEngine::Entity &entity);
    static Entity DeserializeEntity(const std::shared_ptr<Scene> &scene, const YAML::Node &entity);
};

} // namespace MCEngine