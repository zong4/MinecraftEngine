#pragma once

#include "VertexArray.hpp"

namespace Engine
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray(std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute> &attributes,
                      std::unique_ptr<IndexBuffer> indexBuffer, int instanceCount);
    virtual ~OpenGLVertexArray() override;

public:
    void Render(RendererType renderType = RendererType::Triangles, int vertexCount = 0) const override;

protected:
    void Bind() const override;
    void Unbind() const override;
    void SetVertexAttributes(const std::vector<VertexAttribute> &attributes) override;
};

} // namespace Engine