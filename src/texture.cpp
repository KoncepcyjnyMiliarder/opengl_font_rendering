#include <texture.hpp>
#include <SOIL.h>
#include <stdexcept>

texture::texture(const std::string& texture_filename)
  : tex_(0),
    name_(texture_filename)
{
  int width, height;
  int channels = 0;
  unsigned char* image = SOIL_load_image(texture_filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
  if (image == nullptr)
    throw std::runtime_error(texture_filename + " error loading texture");

  glGenTextures(1, &tex_);
  glBindTexture(GL_TEXTURE_2D, tex_);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);
}

texture::texture(const void* compressed_data, unsigned len)
{
  throw std::logic_error("TODO");
}

texture::texture(const void* bgra_data, unsigned width, unsigned height)
{
  throw std::logic_error("TODO");
}

texture::texture(texture&& rhs)
  : tex_(rhs.tex_),
    name_(std::move(rhs.name_))
{
  rhs.tex_ = 0;
}

texture::~texture()
{
  if (tex_)
    glDeleteTextures(1, &tex_);
}

void texture::activate(GLuint slot) const
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, tex_);
}

const std::string& texture::get_name() const
{
  return name_;
}
