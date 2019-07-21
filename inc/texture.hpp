#pragma once

#include <string>
#include <GL/glew.h>

class texture
{

    GLuint tex_;
    const std::string name_;

  public:

    texture(const std::string& texture_filename);
    texture(const void* compressed_data, unsigned len);
    texture(const void* bgra_data, unsigned width, unsigned height);
    texture(const texture&) = delete;
    texture(texture&& rhs);
    texture& operator=(const texture&) = delete;
    texture& operator=(texture&&) = delete;
    ~texture();

    void activate(GLuint slot) const;
    const std::string& get_name() const;
};