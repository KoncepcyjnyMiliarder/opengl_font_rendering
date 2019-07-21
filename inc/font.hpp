#pragma once

#include <font_loader.hpp>
#include <texture.hpp>
#include <GL/glew.h>
#include <unordered_map>

class font
{

  public:

    struct glyph_data
    {
      float x_left;
      float x_right;
      float y_top;
      float y_bot;
      float u_left;
      float u_right;
      float v_top;
      float v_bot;
      float xadvance;
    };

  private:

    texture font_tex_;
    std::unordered_map<wchar_t, glyph_data> glyph_data_;
    float vertical_advance_; // value of step when goin to next line of text

  public:

    font(const font_loader& font);

    float vertical_advance() const;
    const texture& get_font_tex() const;
    const std::unordered_map<wchar_t, glyph_data>& get_glyph_data() const;

};
