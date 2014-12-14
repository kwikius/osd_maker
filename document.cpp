
#include "document.hpp"
#include <quan/atan2.hpp>

document::document()
: m_map_size{quan::length::mm{500},quan::length::mm{500}}
,m_click_bucket{quan::length::mm{3}}
, m_bitmap_size{12,18}
, m_pixel_size {quan::length::mm{10},quan::length::mm{10}}
{
  uint32_t num_elements = m_bitmap_size.x * m_bitmap_size.y;
   for ( uint32_t i = 0;i < num_elements; ++i){
      uint8_t colour = document::grey;
      pixel_array.push_back(colour);
   }
}


