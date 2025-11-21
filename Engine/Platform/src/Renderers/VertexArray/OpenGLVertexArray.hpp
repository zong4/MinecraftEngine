#pragma once

#include "VertexArray.hpp"

namespace Engine
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray(VertexBuffer &&vertexBuffer, const std::vector<VertexAttribute> &attributes,
                      IndexBuffer &&indexBuffer = IndexBuffer(nullptr, 0), int instanceCount = 1);
    virtual ~OpenGLVertexArray() override;

public:
    void Render(RendererType renderType = RendererType::Triangles, size_t positionCount = 0) const override;

protected:
    void Bind() const override;
    void Unbind() const override;
    void SetVertexAttributes(const std::vector<VertexAttribute> &attributes) override;
};

} // namespace Engine