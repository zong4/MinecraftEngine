#include "CameraSystem.hpp"

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
