#include "RayTracing.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void Engine::RayTracing::RenderScene(const Entity &camera, int raysPerPixel, int rayBounces,
                                     std::vector<glm::vec4> &frameBuffer)
{
    PROFILE_FUNCTION();

    int width = camera.GetComponent<CameraComponent>()->GetWidth();
    int height = camera.GetComponent<CameraComponent>()->GetHeight();
    frameBuffer.resize(width * height);

    int lastPercent = -1;
#pragma omp parallel for schedule(static)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            frameBuffer[y * width + x] = glm::vec4(RenderPixel(camera, x, y, raysPerPixel, rayBounces), 1.0f);
        }

        // Progress logging
        int percent = (y + 1) * 100 / height;
        if (percent != lastPercent)
        {
            lastPercent = percent;
            LOG_ENGINE_TRACE("Ray Tracing Progress: " + std::to_string(percent) + "%");
        }
    }

    SaveImage(std::string(PROJECT_ROOT) + "Assets/RayTracedImage.png", width, height, frameBuffer);
}

void Engine::RayTracing::SaveImage(const std::string &filepath, int width, int height,
                                   const std::vector<glm::vec4> &frameBuffer)
{
    PROFILE_FUNCTION();

    std::vector<uint8_t> imageData(width * height * 4);
    for (int i = 0; i < width * height; i++)
    {
        imageData[i * 4 + 0] = static_cast<uint8_t>(glm::clamp(frameBuffer[i].r, 0.0f, 1.0f) * 255.0f);
        imageData[i * 4 + 1] = static_cast<uint8_t>(glm::clamp(frameBuffer[i].g, 0.0f, 1.0f) * 255.0f);
        imageData[i * 4 + 2] = static_cast<uint8_t>(glm::clamp(frameBuffer[i].b, 0.0f, 1.0f) * 255.0f);
        imageData[i * 4 + 3] = static_cast<uint8_t>(glm::clamp(frameBuffer[i].a, 0.0f, 1.0f) * 255.0f);
    }
    stbi_write_png(filepath.c_str(), width, height, 4, imageData.data(), width * 4);
    LOG_ENGINE_INFO("Saved ray traced image to: " + filepath);
}

glm::vec3 Engine::RayTracing::RenderPixel(const Entity &camera, int x, int y, int raysPerPixel, int rayBounces)
{
    PROFILE_FUNCTION();

    glm::vec3 pixelColor(0.0f);
    auto &&transform = camera.GetComponent<TransformComponent>();
    auto &&cameraComp = camera.GetComponent<CameraComponent>();
    for (int i = 0; i < raysPerPixel; i++)
    {
        float u = (x + Random::GetInstance().NextFloat()) / (float)(cameraComp->GetWidth());
        float v = (y + Random::GetInstance().NextFloat()) / (float)(cameraComp->GetHeight());
        Ray ray(transform->Position, cameraComp->GetRayWorld(u, v));

        // Trace the ray and accumulate color (placeholder logic)
        glm::vec3 rayColor(1.0f); // Replace with actual ray-scene intersection logic
        pixelColor += rayColor;
    }
    pixelColor /= (float)(raysPerPixel);
    return pixelColor;
}