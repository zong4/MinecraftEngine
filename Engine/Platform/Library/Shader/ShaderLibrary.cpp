#include "ShaderLibrary.hpp"

MCEngine::ShaderLibrary &MCEngine::ShaderLibrary::GetInstance()
{
    static ShaderLibrary instance;
    return instance;
}

std::string MCEngine::ShaderLibrary::GetName(const std::shared_ptr<Shader> &shader) const
{
    for (const auto &[name, ptr] : m_Shaders)
    {
        if (ptr == shader)
            return name;
    }
    LOG_ENGINE_WARN("Shader not found in library");
    return "";
}

std::shared_ptr<MCEngine::Shader> MCEngine::ShaderLibrary::GetShader(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Shader not found: " + name);
        return nullptr;
    }
    return m_Shaders[name];
}

void MCEngine::ShaderLibrary::AddShader(const std::string &name, const std::shared_ptr<Shader> &shader)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_ERROR("Shader already exists: " + name);
        return;
    }
    m_Shaders[name] = shader;
    LOG_ENGINE_TRACE("Shader added: " + name);
}

std::shared_ptr<MCEngine::Shader> MCEngine::ShaderLibrary::LoadShader(const std::string &name,
                                                                      const std::string &vertexSource,
                                                                      const std::string &fragmentSource,
                                                                      const std::string &geometrySource)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_ERROR("Shader already exists: " + name);
        return m_Shaders[name];
    }

    auto &&shader = std::make_shared<MCEngine::Shader>(vertexSource, fragmentSource, geometrySource);
    AddShader(name, shader);
    return shader;
}

MCEngine::ShaderLibrary::ShaderLibrary()
{
    ENGINE_PROFILE_FUNCTION();

    std::filesystem::path path(std::string(PROJECT_ROOT) + "/Engine/Resources/Shaders/");
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ERROR("Shader directory does not exist: " + path.string());
        return;
    }

    // Load common vertex shader if exists
    std::string commonVertexSource;
    std::filesystem::path commonVertexPath = path / "Common.vs";
    if (std::filesystem::exists(commonVertexPath))
    {
        std::ifstream commonVertexFile(commonVertexPath);
        commonVertexSource =
            std::string((std::istreambuf_iterator<char>(commonVertexFile)), std::istreambuf_iterator<char>());
    }
    else
    {
        LOG_ENGINE_WARN("Common vertex shader not found: " + commonVertexPath.string());
    }

    // Load all shaders
    for (auto &&entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.path().extension() == ".fs")
        {
            std::string shaderName = entry.path().stem().string();

            std::filesystem::path fragmentPath = entry.path();
            std::ifstream fragmentFile(fragmentPath);
            std::string fragmentSource =
                std::string((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());

            std::string vertexSource = commonVertexSource;
            std::filesystem::path vertexPath = entry.path();
            vertexPath.replace_extension(".vs");
            if (std::filesystem::exists(vertexPath))
            {
                std::ifstream vertexFile(vertexPath);
                vertexSource =
                    std::string((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
            }
            else if (!commonVertexSource.empty())
            {
                LOG_ENGINE_INFO("Vertex shader not found for: " + fragmentPath.string() +
                                ", using common vertex shader");
            }
            else
            {
                LOG_ENGINE_WARN("Vertex shader not found for: " + fragmentPath.string());
                continue;
            }

            std::filesystem::path geometryPath = entry.path();
            geometryPath.replace_extension(".gs");
            if (std::filesystem::exists(geometryPath))
            {
                std::ifstream geometryFile(geometryPath);
                std::string geometrySource =
                    std::string((std::istreambuf_iterator<char>(geometryFile)), std::istreambuf_iterator<char>());
                LoadShader(shaderName, vertexSource, fragmentSource, geometrySource);
            }
            else
            {
                LoadShader(shaderName, vertexSource, fragmentSource);
            }
        }
    }

    LOG_ENGINE_INFO("ShaderLibrary initialized");
}

bool MCEngine::ShaderLibrary::Exists(const std::string &name) const { return m_Shaders.find(name) != m_Shaders.end(); }