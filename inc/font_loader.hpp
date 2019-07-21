#pragma once

#include <string>
#include <vector>

class font_loader
{

  public:

    struct char_data
    {
      unsigned id;
      unsigned short x;
      unsigned short y;
      unsigned short width;
      unsigned short height;
      short xoffset;
      short yoffset;
      short xadvance;
      unsigned char page;
      unsigned char chnl;
    };

  private:

    std::string font_name_;
    std::vector<std::string> pages_;
    std::vector<char_data> char_details_;
    unsigned size_;

  public:

    font_loader(const std::string& fontname);
    const std::string& get_name() const;
    const std::vector<std::string>& get_page_filenames() const;
    const std::vector<char_data>& get_char_details() const;
    unsigned get_texture_size() const;
};