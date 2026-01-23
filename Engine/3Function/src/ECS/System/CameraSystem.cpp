#include "CameraSystem.hpp"

#include "../../Renderer/Library/UniformLibrary.hpp"

Engine::CameraSystem &Engine::CameraSystem::GetInstance()
{
    static CameraSystem instance;
    return instance;
}

void Engine::CameraSystem::Resize(entt::registry &registry, int width, int height)
{
    PROFILE_FUNCTION();

    // Resize all cameras
    auto &&view = registry.view<CameraComponent>();
    for (auto &&entity : view)
    {
        auto &&camera = view.get<CameraComponent>(entity);
        camera.Resize(width, height);
    }
}

void Engine::CameraSystem::Update(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Update all cameras' projection matrices
    auto &&view = registry.view<CameraComponent>();
    for (auto &&entity : view)
    {
        auto &&camera = view.get<CameraComponent>(entity);
        camera.UpdateProjectionMatrix();
    }
}

void Engine::CameraSystem::Upload(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Upload main camera data to uniform buffer
    auto &&view = registry.view<TransformComponent, CameraComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

        if (camera.Primary)
        {
            camera.UpdateProjectionMatrix();
            UniformLibrary::GetInstance().UpdateUniform(
                "UniformBuffer0",
                {
                    {glm::value_ptr(glm::inverse(transform.GetTransformMatrix())), sizeof(glm::mat4), 0}, // View matrix
                    {glm::value_ptr(camera.GetProjectionMatrix()), sizeof(glm::mat4),
                     sizeof(glm::mat4)}, // Projection matrix
                    {glm::value_ptr(transform.Position), sizeof(glm::vec3),
                     sizeof(glm::mat4) + sizeof(glm::mat4)}, // Camera position
                });
        }
    }
}