

#include "document.hpp"

document::document()
: m_map_size{quan::length::mm{500},quan::length::mm{500}}
,m_click_bucket{quan::length::mm{3}}
, m_bitmap_size{12,18}
, m_pixel_size {quan::length::mm{10},quan::length::mm{10}}
 ,m_have_image{false}
{
  uint32_t num_elements = m_bitmap_size.x * m_bitmap_size.y;
   for ( uint32_t i = 0;i < num_elements; ++i){
      uint8_t colour = document::grey;
      pixel_array.push_back(colour);
   }
#if 0
//#################### load from png
   wxString path = wxT("/home/andy/cpp/projects/OSDBitmapMaker/res/C_FreeSerif.png");
   wxImage::AddHandler(new wxPNGHandler);
   m_have_image = image.LoadFile(path);
   if ( m_have_image){
     m_bitmap_size.y = image.GetHeight();
     m_bitmap_size.x = image.GetWidth();
     pixel_array.resize(m_bitmap_size.x * m_bitmap_size.y);
     for ( uint32_t y = 0; y < m_bitmap_size.y; ++y){
       for (uint32_t x = 0; x < m_bitmap_size.x; ++x){
         uint8_t colour = document::transparent;
         if ( !image.IsTransparent(x,y)){
            uint8_t red = image.GetRed(x,y);
            uint8_t green = image.GetGreen(x,y);
            uint8_t blue = image.GetBlue(x,y);
            float mono = (red * 0.2126 + green * 0.7152 + blue * 0.0722)/ 255;
            if ( mono < 1./4){
               colour = document::black;
            }else {
               if (mono < 1./2){
                  colour = document::grey;
               }else {
                  colour = document::white;
               }
            }
         }
         set_pixel_colour({x,y},colour);
       }
      }
   }
//#####################################################
#endif
}


