#include <font_loader.hpp>
#include <fstream>
#include <binary_deserializer.hpp>

font_loader::font_loader(const std::string& fontname)
{
  std::ifstream fnt_file(fontname, std::ios::binary);
  if (!fnt_file)
    throw std::runtime_error(fontname + " font file does not exist");

  std::string fnt_data;
  fnt_file.seekg(0, std::ios::end);
  fnt_data.resize(fnt_file.tellg());
  fnt_file.seekg(0, std::ios::beg);
  fnt_file.read(&fnt_data[0], fnt_data.size());

  binary_deserializer bd(fnt_data);

  int magic;
  bd >> magic;

  if (magic != 0x03464D42)
    throw std::runtime_error(fontname + " invalid font magic");

  char block_id;
  unsigned block_size;
  bd >> block_id >> block_size;

  if (block_id != 1)
    throw std::runtime_error(fontname + " expected header id 1");

  if (block_size < 14)
    throw std::runtime_error(fontname + " header 1 malformed");

  //atm ignoring all the details, not really needed
  bd >> serializer_skip(14) >> [this](const char* data, std::size_t& current_pos)
  {
    font_name_ = data + current_pos;
    current_pos += (font_name_.size() + 1);
  };

  bd >> block_id >> block_size;

  if (block_id != 2)
    throw std::runtime_error(fontname + " expected header id 2");

  if (block_size != 15)
    throw std::runtime_error(fontname + " header 2 malformed");

  unsigned short scale_w, scale_h, page_count;
  bd >> serializer_skip(4) >> scale_w >> scale_h >> page_count >> serializer_skip(5);

  if (page_count != 1)
    throw std::runtime_error(fontname + " multiple fontpages are currently not supported");

  if (scale_w != scale_h)
    throw std::runtime_error(fontname + " texture must be square");

  size_ = scale_w;

  bd >> block_id >> block_size;

  if (block_id != 3)
    throw std::runtime_error(fontname + " expected header id 3");

  if (block_size % page_count)
    throw std::runtime_error(fontname + " header 3 malformed");

  auto page_filename_size = block_size / page_count;
  while (page_count--)
  {
    bd >> [this, page_filename_size](const char* data, std::size_t& current_pos)
    {
      pages_.push_back({ data + current_pos, page_filename_size - 1 });
      current_pos += page_filename_size;
    };
  }

  bd >> block_id >> block_size;

  if (block_id != 4)
    throw std::runtime_error(fontname + " expected header id 4");

  if (block_size % sizeof(char_data))
    throw std::runtime_error(fontname + " header 4 malformed");

  char_details_.resize(block_size / sizeof(char_data));
  for(auto& elem : char_details_)
    bd >> elem;
}

const std::string& font_loader::get_name() const
{
  return font_name_;
}

const std::vector<std::string>& font_loader::get_page_filenames() const
{
  return pages_;
}

const std::vector<font_loader::char_data>& font_loader::get_char_details() const
{
  return char_details_;
}

unsigned font_loader::get_texture_size() const
{
  return size_;
}
