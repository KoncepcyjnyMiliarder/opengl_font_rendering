#include <font.hpp>

font::font(const font_loader& font)
  : font_tex_("rsrc/" + font.get_page_filenames().front()),
    vertical_advance_(0.0f)
{
  const auto& tex_size = font.get_texture_size();
  for (const auto& elem : font.get_char_details())
  {
    float xadvance = elem.xadvance;
    float x_left = (float)elem.xoffset;
    float x_right = x_left + (float)elem.width;
    float y_top = (float)elem.yoffset;
    float y_bot = y_top + (float)elem.height;
    float u_left = elem.x;
    float u_right = elem.x + elem.width;
    float v_top = elem.y;
    float v_bot = elem.y + elem.height;
    u_left /= tex_size;
    u_right /= tex_size;
    v_top /= tex_size;
    v_bot /= tex_size;
    glyph_data_[elem.id] = glyph_data{ x_left, x_right, y_top, y_bot, u_left, u_right, v_top, v_bot, xadvance };
    if (vertical_advance_ < (float)elem.height)
      vertical_advance_ = (float)elem.height;
  }
}

float font::vertical_advance() const
{
  return vertical_advance_;
}

const texture& font::get_font_tex() const
{
  return font_tex_;
}

const std::unordered_map<wchar_t, font::glyph_data>& font::get_glyph_data() const
{
  return glyph_data_;
}
