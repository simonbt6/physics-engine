#include "renderer.hpp"

#include <array>

#include <glm/gtc/matrix_transform.hpp>

#include "render_api.hpp"
#include "buffer/uniform_buffer.hpp"

using namespace Graphics::Render;

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

struct CircleVertex
{
    glm::vec3 WorldPosition;
    glm::vec3 LocalPosition;
    glm::vec4 Color;
    float Thickness;
    float Fade;
};

struct LineVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

constexpr uint32_t MAX_QUADS = 20000;
constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
constexpr uint32_t MAX_TEXTURE_SLOTS = 32;

struct SceneData
{
    ref<VertexArray> quad_vao;
    ref<VertexBuffer> quad_vbo;
    ref<Shader> quad_shader;
    ref<Texture> white_texture;

    ref<VertexArray> circle_vao;
    ref<VertexBuffer> circle_vbo;
    ref<Shader> circle_shader;

    ref<VertexArray> line_vao;
    ref<VertexBuffer> line_vbo;
    ref<Shader> line_shader;

    uint32_t quad_index_count = 0;
    QuadVertex* quad_vertex_buffer_base = nullptr;
    QuadVertex* quad_vertex_buffer_ptr = nullptr;

    uint32_t circle_index_count = 0;
    CircleVertex* circle_vertex_buffer_base = nullptr;
    CircleVertex* circle_vertex_buffer_ptr = nullptr;

    uint32_t line_index_count = 0;
    LineVertex* line_vertex_buffer_base = nullptr;
    LineVertex* line_vertex_buffer_ptr = nullptr;

    float line_width = 2.0f;

    std::array<ref<Texture>, MAX_TEXTURE_SLOTS> texture_slots;
    uint32_t texture_slot_index = 1;

    glm::vec4 quad_vertex_positions[4];

    glm::mat4 ViewProjectionMatrix;
    ref<UniformBuffer> camera_uniform_buffer;
};

static SceneData sceneData;

void Renderer::init()
{
    RenderAPI::init();

    sceneData.quad_vao = CreateRef<VertexArray>();
    sceneData.quad_vbo = CreateRef<VertexBuffer>(MAX_VERTICES * sizeof(QuadVertex));
    sceneData.quad_vbo->setLayout({
        { ShaderDatatype::Float3, "a_Position" },
        { ShaderDatatype::Float4, "a_Color" },
        { ShaderDatatype::Float2, "a_TexCoord" },
        { ShaderDatatype::Float,  "a_TexIndex" },
        { ShaderDatatype::Float,  "a_Tiling" }
    });
    sceneData.quad_vao->addVertexBuffer(sceneData.quad_vbo);
    sceneData.quad_vertex_buffer_base = new QuadVertex[MAX_VERTICES];

    uint32_t* quad_indices = new uint32_t[MAX_INDICES];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < MAX_INDICES; i += 6)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;

        offset += 4;
    }

    ref<IndexBuffer> quad_ibo = CreateRef<IndexBuffer>(quad_indices, MAX_INDICES);
    sceneData.quad_vao->setIndexBuffer(quad_ibo);
    delete[] quad_indices;

    sceneData.circle_vao = CreateRef<VertexArray>();
    sceneData.circle_vbo = CreateRef<VertexBuffer>(MAX_VERTICES * sizeof(CircleVertex));
    sceneData.circle_vbo->setLayout({
        { ShaderDatatype::Float3, "a_WorldPosition" },
        { ShaderDatatype::Float3, "a_LocalPosition" },
        { ShaderDatatype::Float4, "a_Color" },
        { ShaderDatatype::Float2, "a_TexCoord" },
        { ShaderDatatype::Float,  "a_TexIndex" },
        { ShaderDatatype::Float,  "a_Tiling" }
    });
    sceneData.circle_vao->addVertexBuffer(sceneData.circle_vbo);
    sceneData.circle_vao->setIndexBuffer(quad_ibo);
    sceneData.circle_vertex_buffer_base = new CircleVertex[MAX_VERTICES];

    sceneData.line_vao = CreateRef<VertexArray>();
    sceneData.line_vbo = CreateRef<VertexBuffer>(MAX_VERTICES * sizeof(LineVertex));
    sceneData.line_vbo->setLayout({
        { ShaderDatatype::Float3, "a_Position" },
        { ShaderDatatype::Float4, "a_Color" }
    });
    sceneData.line_vao->addVertexBuffer(sceneData.line_vbo);
    sceneData.line_vertex_buffer_base = new LineVertex[MAX_VERTICES];
    
    sceneData.white_texture = CreateRef<Texture>(1, 1);
    uint32_t white_texture_data = 0xFFFFFFFF;
    sceneData.white_texture->setData(&white_texture_data, sizeof(uint32_t));
    
    int32_t samplers[MAX_TEXTURE_SLOTS];
    for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
        samplers[i] = i;

    sceneData.quad_shader   = CreateRef<Shader>("quad", "shader/");
    // sceneData.circle_shader = CreateRef<Shader>("circle", "shader/");
    // sceneData.line_shader   = CreateRef<Shader>("line", "shader/");

    sceneData.texture_slots[0] = sceneData.white_texture;

    sceneData.quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    sceneData.quad_vertex_positions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    sceneData.quad_vertex_positions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    sceneData.quad_vertex_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    sceneData.camera_uniform_buffer = CreateRef<UniformBuffer>(sizeof(glm::mat4), 0);
}

void Renderer::shutdown()
{
    delete[] sceneData.quad_vertex_buffer_base;
}

void Renderer::onWindowResize(uint32_t width, uint32_t height)
{
    RenderAPI::setViewport(0, 0, width, height);
}

void Renderer::begin(const glm::mat4& view)
{
    sceneData.ViewProjectionMatrix = view;
    sceneData.camera_uniform_buffer->setData(&sceneData.ViewProjectionMatrix, sizeof(glm::mat4));

    startBatch();
}

void Renderer::end()
{
    flush();
    startBatch();
}

void Renderer::startBatch()
{
    sceneData.quad_index_count = 0;
    sceneData.quad_vertex_buffer_ptr = sceneData.quad_vertex_buffer_base;

    sceneData.circle_index_count = 0;
    sceneData.circle_vertex_buffer_ptr = sceneData.circle_vertex_buffer_base;

    sceneData.line_index_count = 0;
    sceneData.line_vertex_buffer_ptr = sceneData.line_vertex_buffer_base;

    sceneData.texture_slot_index = 1;
}

void Renderer::flush()
{
    for (uint32_t i = 0; i < sceneData.texture_slot_index; i++)
            sceneData.texture_slots[i]->bind(i);

    if (sceneData.quad_index_count)
    {
        uint32_t size = (uint32_t)((uint8_t*)sceneData.quad_vertex_buffer_ptr - (uint8_t*)sceneData.quad_vertex_buffer_base);
        sceneData.quad_vbo->setData(sceneData.quad_vertex_buffer_base, size);
        
        sceneData.quad_shader->bind();
        RenderAPI::drawIndexed(sceneData.quad_vao, sceneData.quad_index_count);
    }

    if (sceneData.circle_index_count)
    {
        uint32_t size = (uint32_t)((uint8_t*)sceneData.circle_vertex_buffer_ptr - (uint8_t*)sceneData.circle_vertex_buffer_base);
        sceneData.circle_vbo->setData(sceneData.circle_vertex_buffer_base, size);
        
        sceneData.circle_shader->bind();
        RenderAPI::drawIndexed(sceneData.circle_vao, sceneData.circle_index_count);
    }

    if (sceneData.line_index_count)
    {
        uint32_t size = (uint32_t)((uint8_t*)sceneData.line_vertex_buffer_ptr - (uint8_t*)sceneData.line_vertex_buffer_base);
        sceneData.line_vbo->setData(sceneData.line_vertex_buffer_base, size);
        
        sceneData.line_shader->bind();
        RenderAPI::drawIndexed(sceneData.line_vao, sceneData.line_index_count);
    }
}

void Renderer::submit(const ref<Shader>& shader, const ref<VertexArray>& vao, const glm::mat4& transform)
{
    shader->bind();
    shader->setMat4("u_ViewProjection", sceneData.ViewProjectionMatrix);
    shader->setMat4("u_Transform", transform);

    vao->bind();
    RenderAPI::drawIndexed(vao);
}



/**
 * 2D Renderer
 * 
 */

void Renderer::drawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
{
    glm::mat4 transform = 
        glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f}) 
        * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
    drawQuad(transform, color);
}

void Renderer::drawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
{
    glm::mat4 transform = 
        glm::translate(glm::mat4(1.0f), position) 
        * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
    drawQuad(transform, color);
}

void Renderer::drawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const ref<Texture>& texture, float tiling, const glm::vec4& tint)
{
    glm::mat4 transform = 
        glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f}) 
        * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
    Renderer::drawQuad(transform, texture, tiling, tint);
}

void Renderer::drawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const ref<Texture>& texture, float tiling, const glm::vec4& tint)
{
    glm::mat4 transform = 
        glm::translate(glm::mat4(1.0f), position) 
        * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
    Renderer::drawQuad(transform, texture, tiling, tint);
}

void Renderer::drawQuad(const glm::mat4& transform, const glm::vec4& color)
{
    constexpr glm::vec2 texCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    constexpr float tiling = 1.0f;

    if (sceneData.quad_index_count >= MAX_INDICES)
        Renderer::end();
    
    for (size_t i = 0; i < 4; i++)
    {
        sceneData.quad_vertex_buffer_ptr->Position = transform * sceneData.quad_vertex_positions[i];
        sceneData.quad_vertex_buffer_ptr->Color = color;
        sceneData.quad_vertex_buffer_ptr->TexCoord = texCoords[i];
        sceneData.quad_vertex_buffer_ptr->TexIndex = 0.0f;
        sceneData.quad_vertex_buffer_ptr->TilingFactor = tiling;
        sceneData.quad_vertex_buffer_ptr++;
    }
    
    sceneData.quad_index_count += 6;
}

void Renderer::drawQuad(const glm::mat4& transform, const ref<Texture>& texture, float tiling, const glm::vec4& tint)
{
    constexpr glm::vec2 texCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    if (sceneData.quad_index_count >= MAX_INDICES)
        Renderer::end();

    float texIndex = 0.0f;
    for (uint32_t i = 1; i < sceneData.texture_slot_index; i++)
    {
        if (*sceneData.texture_slots[i] == *texture)
        {
            texIndex = (float)i;
            break;
        }
    }

    if (!texIndex)
    {
        if (sceneData.texture_slot_index >= MAX_TEXTURE_SLOTS)
            Renderer::end();

        texIndex = (float)sceneData.texture_slot_index;
        sceneData.texture_slots[sceneData.texture_slot_index] = texture;
        sceneData.texture_slot_index++;
    }
    
    for (size_t i = 0; i < 4; i++)
    {
        sceneData.quad_vertex_buffer_ptr->Position = transform * sceneData.quad_vertex_positions[i];
        sceneData.quad_vertex_buffer_ptr->Color = tint;
        sceneData.quad_vertex_buffer_ptr->TexCoord = texCoords[i];
        sceneData.quad_vertex_buffer_ptr->TexIndex = texIndex;
        sceneData.quad_vertex_buffer_ptr->TilingFactor = tiling;
        sceneData.quad_vertex_buffer_ptr++;
    }
    
    sceneData.quad_index_count += 6;
}

void Renderer::drawCircle(const glm::mat4& transform, const glm::vec4& color)
{

}

void Renderer::drawCircle(const glm::mat4& transform, const ref<Texture>& texture, float tiling, const glm::vec4& color)
{

}