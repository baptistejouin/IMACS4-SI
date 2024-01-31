#include <glm/glm.hpp>
#include <glpp/glpp.hpp>

class VAO {
public:
    VAO()
    {
        glGenVertexArrays(1, &_id);
    }
    ~VAO()
    {
        glDeleteVertexArrays(1, &_id);
    }

    // Delete the copy constructor and the copy operator
    VAO(const VAO&)            = delete;
    VAO& operator=(const VAO&) = delete;

    // Move constructor and move operator
    VAO(VAO&& other) noexcept // Move constructor
        : _id{other._id}
    {
        other._id = 0; // Make sure that other won't delete the _id we just copied
    }
    VAO& operator=(VAO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                                  // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteVertexArrays(1, &_id); // Delete the _id of this object
            _id       = other._id;         // Copy the object
            other._id = 0;                 // Make sure that other won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    void bind() const
    {
        glBindVertexArray(_id);
    }
    void unbind() const
    {
        glBindVertexArray(0);
    }

private:
    GLuint _id; // Use 'GLuint' instead of 'GLUint'
};