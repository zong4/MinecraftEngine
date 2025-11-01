#include "SceneSerializer.hpp"

#include <yaml-cpp/yaml.h>

namespace YAML
{

// clang-format off
template <>
// clang-format on
struct convert<glm::vec3>
{
    static Node encode(const glm::vec3 &rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }
    static bool decode(const Node &node, glm::vec3 &rhs)
    {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

// clang-format off
template <>
// clang-format on
struct convert<glm::vec4>
{
    static Node encode(const glm::vec4 &rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }
    static bool decode(const Node &node, glm::vec4 &rhs)
    {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};

// clang-format off
template <>
// clang-format on
struct convert<glm::mat4>
{
    static Node encode(const glm::mat4 &rhs)
    {
        Node node;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                node.push_back(rhs[i][j]);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }
    static bool decode(const Node &node, glm::mat4 &rhs)
    {
        if (!node.IsSequence() || node.size() != 16)
            return false;

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                rhs[i][j] = node[i * 4 + j].as<float>();
        return true;
    }
};

} // namespace YAML

void MCEngine::SceneSerializer::Serialize(const std::shared_ptr<Scene> &scene, const std::string &filepath)
{
    ENGINE_PROFILE_FUNCTION();

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << scene->GetName();

    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    auto &&registry = scene->GetRegistry();
    for (entt::entity entityID : registry.view<entt::entity>())
    {
        if (entityID == entt::null)
            LOG_ENGINE_ERROR("Entity is null, but in registry");
        if (!registry.get<MCEngine::RelationshipComponent>(entityID).GetParent())
            SerializeEntity(out, {entityID, &registry});
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;
    std::ofstream fout(filepath);
    fout << out.c_str();
}

bool MCEngine::SceneSerializer::Deserialize(std::shared_ptr<Scene> scene, const std::string &filepath)
{
    ENGINE_PROFILE_FUNCTION();

    YAML::Node data = YAML::LoadFile(filepath);
    if (!data["Scene"])
        LOG_ENGINE_ERROR("Failed to load scene: " + filepath);

    std::string sceneName = data["Scene"].as<std::string>();
    scene->SetName(sceneName);
    LOG_ENGINE_TRACE("Deserializing scene: " + sceneName);

    auto &&entitiesData = data["Entities"];
    if (entitiesData)
    {
        for (auto &&entityData : entitiesData)
        {
            DeserializeEntity(scene, entityData);
        }
    }
    return true;
}

void MCEngine::SceneSerializer::SerializeEntity(YAML::Emitter &out, MCEngine::Entity entity)
{
    ENGINE_PROFILE_FUNCTION();

    out << YAML::BeginMap;

    // Entity ID
    out << YAML::Key << "Entity" << YAML::Value << static_cast<uint32_t>(entity.GetHandle());

    // TagComponent
    if (entity.HasComponent<MCEngine::TagComponent>())
    {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;

        auto &&tagComponent = entity.GetComponent<MCEngine::TagComponent>();
        out << YAML::Key << "Tag" << YAML::Value << tagComponent.GetTag();

        out << YAML::EndMap;
    }

    // TransformComponent
    if (entity.HasComponent<MCEngine::TransformComponent>())
    {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;

        auto &&transformComponent = entity.GetComponent<MCEngine::TransformComponent>();
        out << YAML::Key << "Position" << YAML::Value << (YAML::Node)transformComponent.GetPosition();
        out << YAML::Key << "Rotation" << YAML::Value << (YAML::Node)transformComponent.GetRotation();
        out << YAML::Key << "Scale" << YAML::Value << (YAML::Node)transformComponent.GetScale();

        out << YAML::EndMap;
    }

    // CameraComponent
    if (entity.HasComponent<MCEngine::CameraComponent>())
    {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;

        // Common
        auto &&cameraComponent = entity.GetComponent<MCEngine::CameraComponent>();
        out << YAML::Key << "Type" << YAML::Value << (int)cameraComponent.GetType();

        // Orthographic
        out << YAML::Key << "Width" << YAML::Value << cameraComponent.GetWidth();
        out << YAML::Key << "Height" << YAML::Value << cameraComponent.GetHeight();
        out << YAML::Key << "Scale" << YAML::Value << cameraComponent.GetScale();

        // Perspective
        out << YAML::Key << "FOV" << YAML::Value << cameraComponent.GetFOV();
        out << YAML::Key << "NearClip" << YAML::Value << cameraComponent.GetNearClip();
        out << YAML::Key << "FarClip" << YAML::Value << cameraComponent.GetFarClip();

        out << YAML::EndMap;
    }

    // SpriteRendererComponent
    if (entity.HasComponent<MCEngine::SpriteRendererComponent>())
    {
        out << YAML::Key << "SpriteRendererComponent";
        out << YAML::BeginMap;

        auto &spriteRendererComponent = entity.GetComponent<MCEngine::SpriteRendererComponent>();
        out << YAML::Key << "Color" << YAML::Value << (YAML::Node)spriteRendererComponent.Color;
        out << YAML::Key << "Texture" << YAML::Value
            << MCEngine::TextureLibrary::GetInstance().GetName(spriteRendererComponent.TextureInstance);

        out << YAML::EndMap;
    }

    // MeshRendererComponent
    if (entity.HasComponent<MCEngine::MeshRendererComponent>())
    {
        out << YAML::Key << "MeshRendererComponent";
        out << YAML::BeginMap;

        auto &meshRendererComponent = entity.GetComponent<MCEngine::MeshRendererComponent>();

        // out << YAML::Key << "Shader" << YAML::Value
        //     << MCEngine::ShaderLibrary::GetInstance().GetName(meshRendererComponent.GetShader());

        out << YAML::Key << "Material";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "Color" << YAML::Value << (YAML::Node)meshRendererComponent.MaterialInstance.Color;
            out << YAML::Key << "AmbientStrength" << YAML::Value
                << meshRendererComponent.MaterialInstance.AmbientStrength;
            out << YAML::Key << "DiffuseStrength" << YAML::Value
                << meshRendererComponent.MaterialInstance.DiffuseStrength;
            out << YAML::Key << "SpecularStrength" << YAML::Value
                << meshRendererComponent.MaterialInstance.SpecularStrength;
            out << YAML::Key << "Shininess" << YAML::Value << meshRendererComponent.MaterialInstance.Shininess;
        }
        out << YAML::EndMap;

        out << YAML::EndMap;
    }

    // LightComponent
    if (entity.HasComponent<MCEngine::LightComponent>())
    {
        out << YAML::Key << "LightComponent";
        out << YAML::BeginMap;

        // Directional
        auto &lightComponent = entity.GetComponent<MCEngine::LightComponent>();
        out << YAML::Key << "Type" << YAML::Value << (int)lightComponent.GetType();
        out << YAML::Key << "Color" << YAML::Value << (YAML::Node)lightComponent.Color;
        out << YAML::Key << "Intensity" << YAML::Value << lightComponent.Intensity;

        // Point
        out << YAML::Key << "Constant" << YAML::Value << lightComponent.Constant;
        out << YAML::Key << "Linear" << YAML::Value << lightComponent.Linear;
        out << YAML::Key << "Quadratic" << YAML::Value << lightComponent.Quadratic;

        // Spot
        out << YAML::Key << "InnerAngle" << YAML::Value << lightComponent.InnerAngle;
        out << YAML::Key << "OuterAngle" << YAML::Value << lightComponent.OuterAngle;

        out << YAML::EndMap;
    }

    // SkyboxComponent
    if (entity.HasComponent<MCEngine::SkyboxComponent>())
    {
        out << YAML::Key << "SkyboxComponent";
        out << YAML::BeginMap;

        auto &skyboxComponent = entity.GetComponent<MCEngine::SkyboxComponent>();
        out << YAML::Key << "Texture" << YAML::Value << skyboxComponent.TextureCubeName;

        out << YAML::EndMap;
    }

    // RelationshipComponent
    if (entity.HasComponent<MCEngine::RelationshipComponent>())
    {
        out << YAML::Key << "RelationshipComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;
        auto &relationshipComponent = entity.GetComponent<MCEngine::RelationshipComponent>();
        for (auto child : relationshipComponent.GetChildren())
            SerializeEntity(out, child);
        out << YAML::EndSeq;

        out << YAML::EndMap;
    }

    out << YAML::EndMap;
}

MCEngine::Entity MCEngine::SceneSerializer::DeserializeEntity(std::shared_ptr<Scene> scene, YAML::Node &entity)
{
    ENGINE_PROFILE_FUNCTION();

    // Entity ID
    uint32_t uuid = entity["Entity"].as<uint32_t>();

    // TagComponent
    std::string name;
    const auto &tagComponentData = entity["TagComponent"];
    const auto &transformComponentData = entity["TransformComponent"];
    if (tagComponentData)
    {
        name = tagComponentData["Tag"].as<std::string>();
    }
    Entity deserializedEntity =
        scene->AddEmptyEntity(name, TransformComponent(transformComponentData["Position"].as<glm::vec3>(),
                                                       transformComponentData["Rotation"].as<glm::vec3>(),
                                                       transformComponentData["Scale"].as<glm::vec3>()));
    LOG_ENGINE_TRACE("Deserialized entity with ID = " + std::to_string(uuid) + ", name = " + name);

    const auto &cameraComponentData = entity["CameraComponent"];
    if (cameraComponentData)
    {
        auto &&cameraComponent = deserializedEntity.AddComponent<CameraComponent>(
            (CameraType)cameraComponentData["Type"].as<int>(), cameraComponentData["Width"].as<float>(),
            cameraComponentData["Height"].as<float>(), cameraComponentData["FOV"].as<float>(),
            cameraComponentData["NearClip"].as<float>(), cameraComponentData["FarClip"].as<float>());
        cameraComponent.SetScale(cameraComponentData["Scale"].as<float>());

        // todo
        scene->SetMainCamera(deserializedEntity);
    }

    const auto &spriteRendererComponentData = entity["SpriteRendererComponent"];
    if (spriteRendererComponentData)
    {
        deserializedEntity.AddComponent<SpriteRendererComponent>(
            MCEngine::TextureLibrary::GetInstance().GetTexture2D(
                spriteRendererComponentData["Texture"].as<std::string>()),
            spriteRendererComponentData["Color"].as<glm::vec4>());
    }

    const auto &meshRendererComponentData = entity["MeshRendererComponent"];
    if (meshRendererComponentData)
    {
        const auto &materialData = meshRendererComponentData["Material"];
        auto &&meshRendererComponent = deserializedEntity.AddComponent<MeshRendererComponent>(
            Material(materialData["Color"].as<glm::vec4>(), materialData["AmbientStrength"].as<float>(),
                     materialData["DiffuseStrength"].as<float>(), materialData["SpecularStrength"].as<float>(),
                     materialData["Shininess"].as<float>()));
    }

    const auto &lightComponentData = entity["LightComponent"];
    if (lightComponentData)
    {
        auto &lightComponent =
            deserializedEntity.AddComponent<LightComponent>((LightType)lightComponentData["Type"].as<int>());
        lightComponent.Color = lightComponentData["Color"].as<glm::vec3>();
        lightComponent.Intensity = lightComponentData["Intensity"].as<float>();
        lightComponent.SetAttenuation(lightComponentData["Constant"].as<float>(),
                                      lightComponentData["Linear"].as<float>(),
                                      lightComponentData["Quadratic"].as<float>());
        lightComponent.SetAngles(lightComponentData["InnerAngle"].as<float>(),
                                 lightComponentData["OuterAngle"].as<float>());
    }

    const auto &skyboxComponentData = entity["SkyboxComponent"];
    if (skyboxComponentData)
    {
        deserializedEntity.AddComponent<SkyboxComponent>(skyboxComponentData["Texture"].as<std::string>());
    }

    const auto &relationshipComponentData = entity["RelationshipComponent"];
    if (relationshipComponentData)
    {
        for (auto child : relationshipComponentData["Children"])
        {
            Entity childEntity = DeserializeEntity(scene, child);
            if (childEntity)
            {
                deserializedEntity.GetComponent<RelationshipComponent>().AddChild(childEntity);
                childEntity.GetComponent<RelationshipComponent>().SetParent(deserializedEntity);
            }
        }
    }

    return deserializedEntity;
}