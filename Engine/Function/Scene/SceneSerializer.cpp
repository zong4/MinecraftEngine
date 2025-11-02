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
    {
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        auto &&registry = scene->GetRegistry();
        for (entt::entity entity : registry.view<entt::entity>())
        {
            if (entity == entt::null)
            {
                LOG_ENGINE_ERROR("Entity is null, but in registry");
                continue;
            }

            if (!registry.get<MCEngine::RelationshipComponent>(entity).GetParent())
                SerializeEntity(out, {entity, &registry});
        }
        out << YAML::EndSeq;
    }

    out << YAML::EndMap;
    std::ofstream fout(filepath);
    fout << out.c_str();
}

bool MCEngine::SceneSerializer::Deserialize(const std::shared_ptr<Scene> &scene, const std::string &filepath)
{
    ENGINE_PROFILE_FUNCTION();

    YAML::Node data = YAML::LoadFile(filepath);
    if (!data["Scene"])
    {
        LOG_ENGINE_ERROR("Failed to load scene: " + filepath);
        return false;
    }

    std::string sceneName = data["Scene"].as<std::string>();
    scene->SetName(sceneName);

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

void MCEngine::SceneSerializer::SerializeEntity(YAML::Emitter &out, const MCEngine::Entity &entity)
{
    ENGINE_PROFILE_FUNCTION();

    out << YAML::BeginMap;

    // Entity ID
    out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity.GetHandle();

    // TagComponent
    auto &&tagComponent = entity.GetComponent<MCEngine::TagComponent>();
    if (tagComponent)
    {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;
        out << YAML::Key << "Tag" << YAML::Value << tagComponent->Tag;
        out << YAML::EndMap;
    }

    // TransformComponent
    auto &&transformComponent = entity.GetComponent<MCEngine::TransformComponent>();
    if (transformComponent)
    {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;
        out << YAML::Key << "Position" << YAML::Value << (YAML::Node)transformComponent->Position;
        out << YAML::Key << "Rotation" << YAML::Value << (YAML::Node)transformComponent->GetRotationEuler();
        out << YAML::Key << "Scale" << YAML::Value << (YAML::Node)transformComponent->Scale;
        out << YAML::EndMap;
    }

    // CameraComponent
    auto &&cameraComponent = entity.GetComponent<MCEngine::CameraComponent>();
    if (cameraComponent)
    {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;

        // Common
        auto &&cameraComponent = entity.GetComponent<MCEngine::CameraComponent>();
        out << YAML::Key << "Type" << YAML::Value << (int)cameraComponent->Type;
        out << YAML::Key << "Primary" << YAML::Value << cameraComponent->Primary;
        out << YAML::Key << "BackgroundColor" << YAML::Value << (YAML::Node)cameraComponent->BackgroundColor;

        // Orthographic
        out << YAML::Key << "Width" << YAML::Value << cameraComponent->GetWidth();
        out << YAML::Key << "Height" << YAML::Value << cameraComponent->GetHeight();
        out << YAML::Key << "Scale" << YAML::Value << cameraComponent->Scale;

        // Perspective
        out << YAML::Key << "FOV" << YAML::Value << cameraComponent->FOV;
        out << YAML::Key << "NearClip" << YAML::Value << cameraComponent->NearClip;
        out << YAML::Key << "FarClip" << YAML::Value << cameraComponent->FarClip;

        out << YAML::EndMap;
    }

    // SpriteRendererComponent
    auto &&spriteRendererComponent = entity.GetComponent<MCEngine::SpriteRendererComponent>();
    if (spriteRendererComponent)
    {
        out << YAML::Key << "SpriteRendererComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "Texture" << YAML::Value
            << MCEngine::TextureLibrary::GetInstance().GetName(spriteRendererComponent->TextureInstance);
        out << YAML::Key << "Color" << YAML::Value << (YAML::Node)spriteRendererComponent->Color;

        out << YAML::EndMap;
    }

    // MeshRendererComponent
    auto &&meshRendererComponent = entity.GetComponent<MCEngine::MeshRendererComponent>();
    if (meshRendererComponent)
    {
        out << YAML::Key << "MeshRendererComponent";
        out << YAML::BeginMap;

        // out << YAML::Key << "Shader" << YAML::Value
        //     << MCEngine::ShaderLibrary::GetInstance().GetName(meshRendererComponent.GetShader());

        out << YAML::Key << "Material";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "Color" << YAML::Value << (YAML::Node)meshRendererComponent->MaterialInstance.Color;
            out << YAML::Key << "AmbientStrength" << YAML::Value
                << meshRendererComponent->MaterialInstance.AmbientStrength;
            out << YAML::Key << "DiffuseStrength" << YAML::Value
                << meshRendererComponent->MaterialInstance.DiffuseStrength;
            out << YAML::Key << "SpecularStrength" << YAML::Value
                << meshRendererComponent->MaterialInstance.SpecularStrength;
            out << YAML::Key << "Shininess" << YAML::Value << meshRendererComponent->MaterialInstance.Shininess;
        }
        out << YAML::EndMap;

        out << YAML::EndMap;
    }

    // LightComponent
    auto &&lightComponent = entity.GetComponent<MCEngine::LightComponent>();
    if (lightComponent)
    {
        out << YAML::Key << "LightComponent";
        out << YAML::BeginMap;

        // Directional
        out << YAML::Key << "Type" << YAML::Value << (int)lightComponent->GetType();
        out << YAML::Key << "Color" << YAML::Value << (YAML::Node)lightComponent->Color;
        out << YAML::Key << "Intensity" << YAML::Value << lightComponent->Intensity;

        // Point
        out << YAML::Key << "Constant" << YAML::Value << lightComponent->Constant;
        out << YAML::Key << "Linear" << YAML::Value << lightComponent->Linear;
        out << YAML::Key << "Quadratic" << YAML::Value << lightComponent->Quadratic;

        // Spot
        out << YAML::Key << "InnerAngle" << YAML::Value << lightComponent->InnerAngle;
        out << YAML::Key << "OuterAngle" << YAML::Value << lightComponent->OuterAngle;

        out << YAML::EndMap;
    }

    // SkyboxComponent
    auto &&skyboxComponent = entity.GetComponent<MCEngine::SkyboxComponent>();
    if (skyboxComponent)
    {
        out << YAML::Key << "SkyboxComponent";
        out << YAML::BeginMap;
        out << YAML::Key << "Texture" << YAML::Value << skyboxComponent->TextureCubeName;
        out << YAML::EndMap;
    }

    // RelationshipComponent
    auto &&relationshipComponent = entity.GetComponent<MCEngine::RelationshipComponent>();
    if (relationshipComponent)
    {
        out << YAML::Key << "RelationshipComponent";
        out << YAML::BeginMap;
        out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;
        for (auto child : relationshipComponent->GetChildren())
            SerializeEntity(out, child);
        out << YAML::EndSeq;
        out << YAML::EndMap;
    }

    out << YAML::EndMap;
}

MCEngine::Entity MCEngine::SceneSerializer::DeserializeEntity(const std::shared_ptr<Scene> &scene,
                                                              const YAML::Node &entity)
{
    ENGINE_PROFILE_FUNCTION();

    // Entity ID
    uint32_t uuid = entity["Entity"].as<uint32_t>();

    // TagComponent
    const auto &tagComponentData = entity["TagComponent"];
    std::string name = tagComponentData["Tag"].as<std::string>();

    // TransformComponent
    const auto &transformComponentData = entity["TransformComponent"];
    Entity deserializedEntity =
        scene->AddEmptyEntity(name, TransformComponent(transformComponentData["Position"].as<glm::vec3>(),
                                                       transformComponentData["Rotation"].as<glm::vec3>(),
                                                       transformComponentData["Scale"].as<glm::vec3>()));
    LOG_ENGINE_TRACE("Deserialized entity with ID = " + std::to_string(uuid) + ", name = " + name);

    // CameraComponent
    const auto &cameraComponentData = entity["CameraComponent"];
    if (cameraComponentData)
    {
        deserializedEntity.AddComponent<CameraComponent>(
            (CameraType)cameraComponentData["Type"].as<int>(), cameraComponentData["BackgroundColor"].as<glm::vec4>(),
            cameraComponentData["Width"].as<float>(), cameraComponentData["Height"].as<float>(),
            cameraComponentData["FOV"].as<float>(), cameraComponentData["NearClip"].as<float>(),
            cameraComponentData["FarClip"].as<float>());
        deserializedEntity.GetComponent<CameraComponent>()->Scale = cameraComponentData["Scale"].as<float>();

        if (cameraComponentData["Primary"].as<bool>())
            scene->SetMainCamera(deserializedEntity);
    }

    // SpriteRendererComponent
    const auto &spriteRendererComponentData = entity["SpriteRendererComponent"];
    if (spriteRendererComponentData)
    {
        deserializedEntity.AddComponent<SpriteRendererComponent>(
            MCEngine::TextureLibrary::GetInstance().GetTexture2D(
                spriteRendererComponentData["Texture"].as<std::string>()),
            spriteRendererComponentData["Color"].as<glm::vec4>());
    }

    // MeshRendererComponent
    const auto &meshRendererComponentData = entity["MeshRendererComponent"];
    if (meshRendererComponentData)
    {
        const auto &materialData = meshRendererComponentData["Material"];
        deserializedEntity.AddComponent<MeshRendererComponent>(
            Material(materialData["Color"].as<glm::vec4>(), materialData["AmbientStrength"].as<float>(),
                     materialData["DiffuseStrength"].as<float>(), materialData["SpecularStrength"].as<float>(),
                     materialData["Shininess"].as<float>()));
    }

    // LightComponent
    const auto &lightComponentData = entity["LightComponent"];
    if (lightComponentData)
    {
        deserializedEntity.AddComponent<LightComponent>(
            (LightType)lightComponentData["Type"].as<int>(), lightComponentData["Color"].as<glm::vec3>(),
            lightComponentData["Intensity"].as<float>(), lightComponentData["Constant"].as<float>(),
            lightComponentData["Linear"].as<float>(), lightComponentData["Quadratic"].as<float>(),
            lightComponentData["InnerAngle"].as<float>(), lightComponentData["OuterAngle"].as<float>());
    }

    // SkyboxComponent
    const auto &skyboxComponentData = entity["SkyboxComponent"];
    if (skyboxComponentData)
    {
        deserializedEntity.AddComponent<SkyboxComponent>(skyboxComponentData["Texture"].as<std::string>());
    }

    // RelationshipComponent
    const auto &relationshipComponentData = entity["RelationshipComponent"];
    if (relationshipComponentData)
    {
        for (const auto &childData : relationshipComponentData["Children"])
        {
            Entity childEntity = DeserializeEntity(scene, childData);
            if (childEntity)
                RelationshipComponent::SetParentChild(deserializedEntity, childEntity);
        }
    }

    return deserializedEntity;
}