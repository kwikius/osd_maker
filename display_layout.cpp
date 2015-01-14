#include "app.h"
#include "display_layout.hpp"
#include "document.hpp"

wxImage const & display_layout::get_image()const
{
   return m_image;
}

void display_layout::set_pixel(pxp const & px,colour c)
{
   switch (c) {
      case colour::black:
         m_image.SetRGB (px.x, px.y, 0, 0, 0);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_OPAQUE);
         break;
      case colour::white:
         m_image.SetRGB (px.x, px.y, 255, 255, 255);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_OPAQUE);
         break;
      case colour::grey:
         m_image.SetRGB (px.x, px.y, 127, 127, 127);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_OPAQUE);
         break;
      case colour::transparent:
         m_image.SetRGB (px.x, px.y, 0, 0, 0);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_TRANSPARENT);
         break;
      default:
         break;
   }
}

void display_layout::bitmap_out(pxp const & pos, osd_image* image)
{

   if ( !image){
      return ;
   }
   auto size_px = image->get_size();
   for ( uint32_t y = 0U; y < size_px.y; ++y){
      for ( uint32_t x = 0U; x < size_px.x; ++x){
          pxp out_pos{pos.x + x, pos.y + y};
          colour c = osd_image::colour::transparent;
          image->get_pixel_colour({x,y},c);
          if ( c != osd_image::colour::transparent){
            set_pixel(out_pos,c); 
          }
      }
   }
}

void display_layout::text_out(pxp const & pos_in,std::string const & text, font* font_in)
{
   assert( font_in && __LINE__);
   if ( text == ""){
      return;
   }
   pxp pos = pos_in;
   for (const char* ptr = text.c_str(); *ptr != '\0'; ++ptr) {
        // abc_bitmap<uint8_t>* fontch = get_font_char (*ptr);
         int char_handle =-1;
         font_in->get_handle_at(*ptr,char_handle);
         assert( (char_handle != -1) && __LINE__);
         auto char_bmp = wxGetApp().get_document()->get_image(char_handle);
         if (char_bmp) {
            bitmap_out(pos,char_bmp);
            pos.x += char_bmp->get_size().x;
         }
      }
}

display_layout::display_layout()
{
   if (! m_image.HasAlpha()){
      m_image.SetAlpha();
   }
   if (! wxImage::FindHandler (wxBITMAP_TYPE_PNG)) {
         wxImage::AddHandler (new wxPNGHandler);
   }
   wxImage bkgnd_image{wxT("fpv_background.png")};
   if (!bkgnd_image.IsOk()){
      wxMessageBox(wxT("bk fail"));
   }
   m_background_image = bkgnd_image;
   m_image = bkgnd_image;
};

void display_layout::clear()
{
   m_image = m_background_image;
}
