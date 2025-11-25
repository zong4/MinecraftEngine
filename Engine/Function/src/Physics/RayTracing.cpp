#include "RayTracing.hpp"

void Engine::RayTracing::RenderScene(const Entity &camera, int raysPerPixel, int rayBounces,
                                     std::vector<glm::vec4> &frameBuffer)
{
    PROFILE_FUNCTION();

    int width = camera.GetComponent<CameraComponent>()->GetWidth();
    int height = camera.GetComponent<CameraComponent>()->GetHeight();
    frameBuffer.resize(width * height);

#pragma omp parallel for schedule(static)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            frameBuffer[y * width + x] = glm::vec4(RenderPixel(camera, x, y, raysPerPixel, rayBounces), 1.0f);
        }

        // Show progress every 10 rows
        if (y % 10 == 0)
            LOG_ENGINE_TRACE("Ray tracing progress: " + std::to_string((y + 1) * 100 / height) + "%");
    }
}

glm::vec3 Engine::RayTracing::RenderPixel(const Entity &camera, int x, int y, int raysPerPixel, int rayBounces)
{
    PROFILE_FUNCTION();

    glm::vec3 pixelColor(0.0f);
    auto &&transform = camera.GetComponent<TransformComponent>();
    auto &&cameraComp = camera.GetComponent<CameraComponent>();
    for (int i = 0; i < raysPerPixel; i++)
    {
        float u = (x + Engine::Random::RandomFloat()) / (float)(cameraComp->GetWidth());
        float v = (y + Engine::Random::RandomFloat()) / (float)(cameraComp->GetHeight());
        Ray ray(transform->Position, cameraComp->GetRayWorld(u, v));

        // Trace the ray and accumulate color (placeholder logic)
        glm::vec3 rayColor(1.0f); // Replace with actual ray-scene intersection logic
        pixelColor += rayColor;
    }
    pixelColor /= (float)(raysPerPixel);
    return pixelColor;
}