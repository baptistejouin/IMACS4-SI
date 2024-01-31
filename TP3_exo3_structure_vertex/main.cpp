#include <utils/ibo.h>
#include <glm/glm.hpp>
#include "glimac/default_shader.hpp"
#include "p6/p6.h"

/**
 * @docs: https://julesfouchy.github.io/Learn--OpenGL/TP3/dessiner-un-triangle-blanc#le-vbo
 */

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX3"}};
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
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Create an array of vertices to draw a triangle
    std::array<Vertex2DColor, 4> vertices = {
        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // Sommet 0
        Vertex2DColor{{+0.5f, -0.5f}, {0.f, 1.f, 0.f}}, // Sommet 1
        Vertex2DColor{{+0.5f, +0.5f}, {0.f, 0.f, 1.f}}, // Sommet 2
        Vertex2DColor{{-0.5f, +0.5f}, {1.f, 1.f, 1.f}}, // Sommet 3
    };

    // Send the vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);

    // Unbind the VBO to avoid modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create a IBO
    IBO ibo{};

    ibo.bind();

    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ibo.unbind();

    // Create a VAO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);

    // Bind the VAO
    glBindVertexArray(vao);

    // Bind the IBO
    ibo.bind();

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // vertex attribute activation
    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), reinterpret_cast<const GLvoid*>(offsetof(Vertex2DColor, position)));

    static constexpr GLuint VERTEX_ATTR_COLOR = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), reinterpret_cast<const GLvoid*>(offsetof(Vertex2DColor, color)));

    // Unbind the VBO to avoid modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind the VAO
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // Free the memory allocated for the VBO
    glDeleteBuffers(1, &vbo);

    // Free the memory allocated for the VAO
    glDeleteVertexArrays(1, &vao);
}
