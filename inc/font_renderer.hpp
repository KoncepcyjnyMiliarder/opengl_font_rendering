#pragma once

#include <font.hpp>
#include <generic_shader.hpp>
#include <cached_text.hpp>

class font_renderer
{
    generic_shader text_shader_;

  public:

    font_renderer();
    font_renderer(const font_renderer&) = delete;

    void use(const glm::mat4& ortho) const;
    void draw_text(const cached_text& text, float screen_x, float screen_y, const glm::vec3& color) const;
};