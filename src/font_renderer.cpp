#include <font_renderer.hpp>
#include <numeric>

font_renderer::font_renderer()
  : text_shader_(R"(rsrc/text.vs)", R"(rsrc/text.fs)")
{
}


void font_renderer::use(const glm::mat4& ortho) const
{
  text_shader_.use();
  text_shader_.set_uint_uniform("symbol_atlas", 0);
  text_shader_.set_mat4_uniform("ortho", ortho);
}

void font_renderer::draw_text(const cached_text& text, float screen_x, float screen_y, const glm::vec3& color) const
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  text_shader_.set_3f_uniform("color", color.x, color.y, color.z);
  text.bind();
  text_shader_.set_2f_uniform("translation", screen_x, screen_y);
  glDrawArrays(GL_TRIANGLES, 0, 6 * text.text().size());
  glDisable(GL_BLEND);
}
