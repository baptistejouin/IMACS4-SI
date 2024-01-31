#include <glm/glm.hpp>
#include <glpp/glpp.hpp>

class VBO {
public:
    VBO()
    {
        glGenBuffers(1, &_id);
    }
    ~VBO()
    {
        glDeleteBuffers(1, &_id);
    }

    // Delete the copy constructor and the copy operator
    VBO(const VBO&)            = delete;
    VBO& operator=(const VBO&) = delete;

    // Move constructor and move operator
    VBO(VBO&& other) noexcept // Move constructor
        : _id{other._id}
    {
        other._id = 0; // Make sure that other won't delete the _id we just copied
    }
    VBO& operator=(VBO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                             // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteBuffers(1, &_id); // Delete the previous object
            _id       = other._id;    // Copy the object
            other._id = 0;            // Make sure that other won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }
    void unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    GLuint _id; // Use 'GLuint' instead of 'GLUint'
};