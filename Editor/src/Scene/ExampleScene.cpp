#include "ExampleScene.hpp"

#include "../Script/PlayerController.hpp"
#include "../Script/RotatingLight.hpp"

Editor::ExampleScene::ExampleScene() : Engine::Scene3D()
{
    PROFILE_FUNCTION();

    // Camera
    auto camera = AddCamera("MainCamera",
                            Engine::TransformComponent(glm::vec3(0.0f, 10.0f, 20.0f), glm::vec3(-30.0f, 0.0f, 0.0f)),
                            Engine::CameraComponent(Engine::CameraType::Perspective));
    SetMainCamera(camera);

    // 2D
    AddSquare("Square", Engine::TransformComponent(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f)));

    // 2D
    AddSquare("02BG", Engine::TransformComponent(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f), glm::vec3(3.0f)),
              Engine::MaterialComponent(Engine::MaterialsManager::GetInstance().GetMaterial("02BG")));

    // Light
    auto light = AddLight("DirectionalLight",
                          Engine::TransformComponent(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(-45.0f, -45.0f, 0.0f)),
                          Engine::LightComponent(Engine::LightType::Directional));
    light.AddComponent<Engine::NativeScriptComponent>();
    light.GetComponent<Engine::NativeScriptComponent>()->Bind<RotatingLight>(light);

    // Underground Generation with Perlin Noise
    int width = 16;
    int height = 24;
    int length = 16;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height / 2; y++)
        {
            for (int z = 0; z < length; z++)
            {
                float density =
                    static_cast<float>(Engine::PerlinNoise::GetInstance().Noise(x * 0.08f, y * 0.08f, z * 0.08f));
                if (density < 0.1f)
                {
                    AddCube(
                        "Stone" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z),
                        Engine::TransformComponent(glm::vec3(x - width / 2, y - height / 2, z - length / 2)),
                        Engine::MaterialComponent(Engine::MaterialsManager::GetInstance().GetMaterial("StoneBlock")));
                }
            }
        }
    }

    // Aboveground Generation with Perlin Noise + FBM
    for (int x = 0; x < width; x++)
    {
        for (int z = 0; z < length; z++)
        {
            float noise = static_cast<float>(Engine::PerlinNoise::GetInstance().FBM(x * 0.05f, z * 0.05f, 4, 2.0, 0.5));
            if (noise > 0.0f)
            {
                for (int y = 0; y < noise * height; y++)
                {
                    AddCube(
                        "Grass" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z),
                        Engine::TransformComponent(glm::vec3(x - width / 2, y, z - length / 2)),
                        Engine::MaterialComponent(Engine::MaterialsManager::GetInstance().GetMaterial("GrassBlock")));
                }
            }
        }
    }

    // // Player
    // auto player = AddCube("Player", Engine::TransformComponent(glm::vec3(0.0f, 3.0f, 0.0f)));
    // player.AddComponent<Engine::AudioComponent>(true, 1.0f);
    // player.AddComponent<Engine::RigidBodyComponent>();
    // player.AddComponent<Engine::NativeScriptComponent>();
    // player.GetComponent<Engine::NativeScriptComponent>()->Bind<PlayerController>(player);
    // player.AddComponent<Engine::LuaScriptComponent>();
    // player.GetComponent<Engine::LuaScriptComponent>()->ScriptPath =
    //     (Engine::GetAssetsDirectory() / "Scripts/Test.lua").string();
}
