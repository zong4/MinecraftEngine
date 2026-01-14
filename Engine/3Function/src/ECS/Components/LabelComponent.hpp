#pragma once

namespace Engine
{

enum class TagType
{
    None = 0,
    Player,
    MainCamera,
    Light,
    Skybox
};

enum class LayerType
{
    Default = 0,
    UI,
    Background,
    Foreground
};

struct LabelComponent
{
    std::string Name;
    TagType Tag;
    LayerType Layer;

public:
    LabelComponent(const std::string &name, TagType tag = TagType::None, LayerType layer = LayerType::Default)
        : Name(name), Tag(tag), Layer(layer)
    {
    }
};

} // namespace Engine