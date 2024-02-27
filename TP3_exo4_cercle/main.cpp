// #include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include "glimac/default_shader.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"
#include "p6/p6.h"
#include "utils/ibo.h"
#include "utils/vao.h"
#include "utils/vbo.h"

/**
 * @docs: https://julesfouchy.github.io/Learn--OpenGL/TP3/dessiner-un-triangle-blanc#le-vbo
 */

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

std::vector<Vertex2DColor> get_disk_vertex(float radius, int slices)
{
    std::vector<Vertex2DColor> vertices;

    for (int i = 0; i < slices; ++i)
    {
        // first point of the triangle
        vertices.push_back({{0.f, 0.f}, {1.f, 1.f, 1.f}});
        // second point of the triangle
        vertices.push_back({{radius * std::cos(2 * glm::pi<float>() * i / slices), radius * std::sin(2 * glm::pi<float>() * i / slices)}, {1.f, 1.f, .5f}});
        // third point of the triangle
        vertices.push_back({{radius * std::cos(2 * glm::pi<float>() * (i + 1) / slices), radius * std::sin(2 * glm::pi<float>() * (i + 1) / slices)}, {.5f, 1.f, 1.f}});
    }

    return vertices;
}

std::vector<uint32_t> get_disk_indices(int slices)
{
    std::vector<uint32_t> indices;

    for (int i = 0; i < slices; ++i)
    {
        // first point of the triangle
        indices.push_back(0);
        // second point of the triangle
        indices.push_back(3 * i + 1);
        // third point of the triangle
        indices.push_back(3 * i + 2);
    }

    return indices;
}

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX4"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Load the shader for the color
    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    // Create a VBO
    VBO vbo{};

    // Bind the VBO
    vbo.bind();

    // Create an array of vertices to draw a quad
    std::vector<Vertex2DColor> vertices = get_disk_vertex(0.5f, 25);

    // Send the vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex2DColor)), vertices.data(), GL_STATIC_DRAW);

    // Unbind the VBO to avoid modification
    vbo.unbind();

    // Create a IBO
    IBO ibo{};

    // Bind the IBO
    ibo.bind();

    // Create an array of indices
    std::vector<uint32_t> indices = get_disk_indices(25);

    // Send the indices to the GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    // Unbind the IBO to avoid modification
    ibo.unbind();

    // Create a VAO
    VAO vao{};

    // Bind the VAO
    vao.bind();

    // Bind the IBO
    ibo.bind();

    // Bind the VBO
    vbo.bind();

    // vertex attribute activation
    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), reinterpret_cast<const GLvoid*>(offsetof(Vertex2DColor, position)));

    static constexpr GLuint VERTEX_ATTR_COLOR = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), reinterpret_cast<const GLvoid*>(offsetof(Vertex2DColor, color)));

    // Unbind the VBO to avoid modification
    vbo.unbind();

    // Unbind the VAO to avoid modification
    vao.unbind();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // Clear the color buffer to avoid accumulation (here we only need to clear the color buffer)
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the VAO
        vao.bind();

        // Use the default shader provided by glimac
        glimac::bind_default_shader();

        // Use the shader (before draw arrays)
        shader.use();

        // Draw the triangle
        // glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

        // Unbind the VAO
        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
