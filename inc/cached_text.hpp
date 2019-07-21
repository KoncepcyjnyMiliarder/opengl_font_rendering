#pragma once

#include <string>
#include <GL/glew.h>
#include <font.hpp>

class cached_text
{
    GLuint vao_, vbo_;
    const font& the_font_;
    std::wstring text_;

    void do_buffer_init();
    void push_letter(const font::glyph_data& glyph, float cursor_x, float cursor_y, std::vector<float>& data_for_gpu);

  public:

    cached_text(const std::wstring& text, const font& fnt);
    cached_text(const std::wstring& text, const font& fnt, float max_width, float max_height);
    ~cached_text();
    cached_text(const cached_text&) = delete;
    cached_text& operator=(const cached_text&) = delete;

    void bind() const;
    const std::wstring& text() const;
};