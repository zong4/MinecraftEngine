#include "RayTracingSystem.hpp"

#include "../../Algorithm/Random.hpp"

Engine::RayTracingSystem &Engine::RayTracingSystem::GetInstance()
{
    static RayTracingSystem instance;
    return instance;
}

Engine::RayTracingSystem::~RayTracingSystem()
{
    if (m_Thread.joinable())
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Running = false;
        }
        m_CV.notify_one();
        m_Thread.join();
    }
}

void Engine::RayTracingSystem::Render(entt::registry &registry, const Entity &camera, int raysPerPixel, int rayBounces)
{
    PROFILE_FUNCTION();

    if (m_Running)
        return;

    // Prepare objects
    m_Objects.clear();
    auto &&view = registry.view<TransformComponent, MeshRendererComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, meshRenderer] = view.get<TransformComponent, MeshRendererComponent>(entity);
        RayTracingObject obj;
        obj.transform = transform.GetTransformMatrix();
        obj.bbox = meshRenderer.WorldBBox;
        m_Objects.push_back(obj);
    }

    // Prepare framebuffer
    auto &&cameraComponent = camera.GetComponent<CameraComponent>();
    if (m_Width != cameraComponent->GetWidth() || m_Height != cameraComponent->GetHeight())
    {
        m_Width = cameraComponent->GetWidth();
        m_Height = cameraComponent->GetHeight();
        m_FrameBuffer.resize(m_Width * m_Height);
    }

    // Start rendering thread
    m_Running = true;
    m_Thread = std::thread([this, camera, raysPerPixel, rayBounces]() {
        int lastPercent = -1;
        for (int y = 0; y < m_Height; y++)
        {
            for (int x = 0; x < m_Width; x++)
            {
                glm::vec3 color = RenderPixel(camera, raysPerPixel, rayBounces, x, y);
                m_FrameBuffer[y * m_Width + x] = glm::vec4(color, 1.0f);
            }

            int percent = (y + 1) * raysPerPixel / m_Height;
            if (percent != lastPercent)
            {
                lastPercent = percent;
                LOG_ENGINE_TRACE("Ray Tracing Progress: " + std::to_string(percent) + "%");
            }
        }

        // Save image after rendering
        {
            // Generate timestamped filename
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::tm tm;
#if defined(_WIN32)
            localtime_s(&tm, &t);
#else
            localtime_r(&t, &tm);
#endif
            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".png";
            std::string filename = std::string(PROJECT_ROOT) + "/Assets/RayTracing_" + ss.str();

            // Save image
            SaveImage(filename);
        }

        // Notify completion
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Running = false;
        }
        m_CV.notify_one();
    });
}

void Engine::RayTracingSystem::SaveImage(const std::string &filepath)
{
    PROFILE_FUNCTION();

    if (m_Running)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_CV.wait(lock, [this]() { return !m_Running.load(); });
    }

    if (m_FrameBuffer.empty())
    {
        LOG_ENGINE_WARN("No image data to save!");
        return;
    }

    // Save using stb_image_write
    std::vector<uint8_t> imageData(m_FrameBuffer.size() * 4);
    for (size_t i = 0; i < m_FrameBuffer.size(); i++)
    {
        imageData[i * 4 + 0] = static_cast<unsigned char>(glm::clamp(m_FrameBuffer[i].r, 0.0f, 1.0f) * 255.0f);
        imageData[i * 4 + 1] = static_cast<unsigned char>(glm::clamp(m_FrameBuffer[i].g, 0.0f, 1.0f) * 255.0f);
        imageData[i * 4 + 2] = static_cast<unsigned char>(glm::clamp(m_FrameBuffer[i].b, 0.0f, 1.0f) * 255.0f);
        imageData[i * 4 + 3] = 255;
    }
    Engine::Texture::SaveImage(filepath, m_Width, m_Height, imageData.data());
}

glm::vec3 Engine::RayTracingSystem::RenderPixel(const Entity &camera, int raysPerPixel, int rayBounces, int x, int y)
{
    PROFILE_FUNCTION();

    auto &&transform = camera.GetComponent<TransformComponent>();
    auto &&cameraComponent = camera.GetComponent<CameraComponent>();

    glm::vec3 finalColor(0.0f);
    for (int i = 0; i < raysPerPixel; i++)
    {
        // Jittered sampling within the pixel
        float u = (x + Random::GetInstance().NextFloat()) / (float)m_Width;
        float v = (y + Random::GetInstance().NextFloat()) / (float)m_Height;

        // Generate ray direction
        glm::vec3 rayDir = cameraComponent->GetDirection(u, v);
        rayDir = glm::normalize(glm::vec3(transform->GetTransformMatrix() * glm::vec4(rayDir, 0.0f)));

        // Simple ray tracing logic (placeholder)
        Ray ray(transform->GetGlobaldPosition(), rayDir);
        for (auto &&obj : m_Objects)
        {
            if (ray.Hit(obj.bbox, 0.001f, FLT_MAX))
            {
                // For simplicity, we just return a solid color for hit objects
                finalColor += glm::vec3(1.0f, 0.0f, 1.0f); // White color for hit
            }
            else
            {
                finalColor += glm::vec3(cameraComponent->BackgroundColor); // Background color
            }
        }
    }

    finalColor /= (float)raysPerPixel;
    return finalColor;
}