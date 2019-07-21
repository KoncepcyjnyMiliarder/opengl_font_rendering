#include <generic_shader.hpp>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

generic_shader::generic_shader(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename)
{
  std::ifstream vs_file(vertex_shader_filename);
  std::ifstream fs_file(fragment_shader_filename);

  if (!vs_file)
    throw std::runtime_error(vertex_shader_filename + " unable to load");
  if (!fs_file)
    throw std::runtime_error(fragment_shader_filename + " unable to load");

  std::stringstream vsss;
  vsss << vs_file.rdbuf();
  auto vertex_contents = vsss.str();

  std::stringstream fsss;
  fsss << fs_file.rdbuf();
  auto fragment_contents = fsss.str();

  GLint success;
  std::string info_log;
  info_log.resize(512);

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* vss = &vertex_contents[0];
  glShaderSource(vertex_shader, 1, &vss, NULL);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex_shader, info_log.size(), NULL, &info_log[0]);
    throw std::runtime_error(info_log + " vertex shader complation failed");
  }

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* fss = &fragment_contents[0];
  glShaderSource(fragment_shader, 1, &fss, NULL);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment_shader, info_log.size(), NULL, &info_log[0]);
    throw std::runtime_error(info_log + " fragment shader complation failed");
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vertex_shader);
  glAttachShader(program_, fragment_shader);
  glLinkProgram(program_);

  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(program_, info_log.size(), NULL, &info_log[0]);
    throw std::runtime_error(info_log + " shader program linking failed");
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

generic_shader::~generic_shader()
{
  glDeleteProgram(program_);
}

void generic_shader::use() const
{
  glUseProgram(program_);
}

void generic_shader::set_mat4_uniform(const std::string& name, const glm::mat4& mat) const
{
  auto location = glGetUniformLocation(program_, name.c_str());
  if (location == -1)
    throw std::runtime_error(name + " uniform not found in the shader");
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void generic_shader::set_mat4_uniform(const std::string& name, const float mat[16]) const
{
  auto location = glGetUniformLocation(program_, name.c_str());
  if (location == -1)
    throw std::runtime_error(name + " uniform not found in the shader");
  glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

void generic_shader::set_mat3_uniform(const std::string& name, const glm::mat3& mat) const
{
  auto location = glGetUniformLocation(program_, name.c_str());
  if (location == -1)
    throw std::runtime_error(name + " uniform not found in the shader");
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void generic_shader::set_uint_uniform(const std::string& name, GLuint val) const
{
  auto location = glGetUniformLocation(program_, name.c_str());
  if (location == -1)
    throw std::runtime_error(name + " uniform not found in the shader");
  glUniform1i(location, val);
}

void generic_shader::set_3f_uniform(const std::string& name, float a, float b, float c) const
{
  auto location = glGetUniformLocation(program_, name.c_str());
  if (location == -1)
    throw std::runtime_error(name + " uniform not found in the shader");
  glUniform3f(location, a, b, c );
}

void generic_shader::set_3f_uniform(const std::string& name, const glm::vec3& vec) const
{
  set_3f_uniform(name, vec.x, vec.y, vec.z);
}

void generic_shader::set_2f_uniform(const std::string& name, float a, float b) const
{
  auto location = glGetUniformLocation(program_, name.c_str());
  if (location == -1)
    throw std::runtime_error(name + " uniform not found in the shader");
  glUniform2f(location, a, b);
}

void generic_shader::set_2f_uniform(const std::string& name, const glm::vec2& vec) const
{
  set_2f_uniform(name, vec.x, vec.y);
}

void generic_shader::set_1f_uniform(const std::string& name, float a) const
{
  auto location = glGetUniformLocation(program_, name.c_str());
  if (location == -1)
    throw std::runtime_error(name + " uniform not found in the shader");
  glUniform1f(location, a);
}
