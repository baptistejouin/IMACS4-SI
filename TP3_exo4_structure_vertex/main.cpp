// #include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include "glimac/default_shader.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"
#include "p6/p6.h"
#include "utils/vbo.h"

/**
 * @docs: https://julesfouchy.github.io/Learn--OpenGL/TP3/dessiner-un-triangle-blanc#le-vbo
 */

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

std::vector<Vertex2DColor> create_disk(float radius, int slices)
{
    std::vector<Vertex2DColor> vertices;

    // Add the center of the disk
    vertices.push_back({{0.f, 0.f}, {1.f, 1.f, 1.f}});

    // Add the vertices of the disk
    for (int i = 0; i < slices; ++i)
    {
        const float angle = i * glm::two_pi<float>() / slices;
        vertices.push_back({{radius * std::cos(angle), radius * std::sin(angle)}, {1.f, 1.f, 1.f}});
    }

    // Add the last vertex to close the disk
    vertices.push_back({{radius, 0.f}, {1.f, 1.f, 1.f}});

    return vertices;
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

    // Create a VAO
    VBO vbo{};

    // Bind the VBO
    vbo.bind();

    // Create an array of vertices to draw a quad
    std::vector<Vertex2DColor> vertices = create_disk(0.5f, 200);

    // Send the vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex2DColor)), vertices.data(), GL_STATIC_DRAW);

    // Unbind the VBO to avoid modification
    vbo.unbind();

    // Create a VAO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);

    // Bind the VAO
    glBindVertexArray(vao);

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
    glBindVertexArray(0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // Clear the color buffer to avoid accumulation (here we only need to clear the color buffer)
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the VAO
        glBindVertexArray(vao);

        // Use the default shader provided by glimac
        glimac::bind_default_shader();

        // Use the shader (before draw arrays)
        shader.use();

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

        // Unbind the VAO
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
