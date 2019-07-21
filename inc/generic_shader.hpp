#pragma once

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class generic_shader
{

    GLuint program_;

  public:

    generic_shader(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename);
    ~generic_shader();
    void use() const;
    void set_mat4_uniform(const std::string& name, const glm::mat4& mat) const;
    void set_mat4_uniform(const std::string& name, const float mat[16]) const;
    void set_mat3_uniform(const std::string& name, const glm::mat3& mat) const;
    void set_uint_uniform(const std::string& name, GLuint val) const;
    void set_3f_uniform(const std::string& name, float a, float b, float c) const;
    void set_3f_uniform(const std::string& name, const glm::vec3& vec) const;
    void set_2f_uniform(const std::string& name, float a, float b) const;
    void set_2f_uniform(const std::string& name, const glm::vec2& vec) const;
    void set_1f_uniform(const std::string& name, float a) const;
};