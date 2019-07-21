#include <cached_text.hpp>

void cached_text::do_buffer_init()
{
  glCreateVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  //pos
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  //uv
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
}

void cached_text::push_letter(const font::glyph_data& glyph, float cursor_x, float cursor_y, std::vector<float>& data_for_gpu)
{
  //bottom left
  data_for_gpu.push_back(glyph.x_left + cursor_x);
  data_for_gpu.push_back(glyph.y_bot + cursor_y);
  data_for_gpu.push_back(glyph.u_left);
  data_for_gpu.push_back(glyph.v_bot);
  //upper left
  data_for_gpu.push_back(glyph.x_left + cursor_x);
  data_for_gpu.push_back(glyph.y_top + cursor_y);
  data_for_gpu.push_back(glyph.u_left);
  data_for_gpu.push_back(glyph.v_top);
  //bottom right
  data_for_gpu.push_back(glyph.x_right + cursor_x);
  data_for_gpu.push_back(glyph.y_bot + cursor_y);
  data_for_gpu.push_back(glyph.u_right);
  data_for_gpu.push_back(glyph.v_bot);
  //upper left
  data_for_gpu.push_back(glyph.x_left + cursor_x);
  data_for_gpu.push_back(glyph.y_top + cursor_y);
  data_for_gpu.push_back(glyph.u_left);
  data_for_gpu.push_back(glyph.v_top);
  //upper right
  data_for_gpu.push_back(glyph.x_right + cursor_x);
  data_for_gpu.push_back(glyph.y_top + cursor_y);
  data_for_gpu.push_back(glyph.u_right);
  data_for_gpu.push_back(glyph.v_top);
  //bottom right
  data_for_gpu.push_back(glyph.x_right + cursor_x);
  data_for_gpu.push_back(glyph.y_bot + cursor_y);
  data_for_gpu.push_back(glyph.u_right);
  data_for_gpu.push_back(glyph.v_bot);
}

cached_text::cached_text(const std::wstring& text, const font& fnt)
  : the_font_(fnt),
    text_(text)
{
  do_buffer_init();
  std::vector<float> data_for_gpu;
  data_for_gpu.reserve(6 * 4 * text.size());
  float advancement = 0.0f;
  for (const auto& letter : text)
  {
    auto& glyph = the_font_.get_glyph_data().at(letter);
    push_letter(glyph, advancement, 0.0f, data_for_gpu);
    advancement += glyph.xadvance;
  }
  glBindVertexArray(vao_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data_for_gpu.size(), data_for_gpu.data(), GL_STATIC_DRAW);
}

cached_text::cached_text(const std::wstring& text, const font& fnt, float max_width, float max_height)
  : the_font_(fnt),
    text_(text)
{
  do_buffer_init();
  std::vector<float> data_for_gpu;
  data_for_gpu.reserve(6 * 4 * text.size());
  float xadvancement = 0.0f;
  float yadvancement = 0.0f;
  for (const auto& letter : text)
  {
    auto& glyph = the_font_.get_glyph_data().at(letter);
    if (xadvancement + glyph.xadvance > max_width)
    {
      xadvancement = 0.0f;
      yadvancement += the_font_.vertical_advance();
      if (yadvancement > max_height)
        break;
    }
    push_letter(glyph, xadvancement, yadvancement, data_for_gpu);
    xadvancement += glyph.xadvance;
  }
  glBindVertexArray(vao_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data_for_gpu.size(), data_for_gpu.data(), GL_STATIC_DRAW);
}

cached_text::~cached_text()
{
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
}

void cached_text::bind() const
{
  the_font_.get_font_tex().activate(0);
  glBindVertexArray(vao_);
}

const std::wstring& cached_text::text() const
{
  return text_;
}
