#include "../display_layout.hpp"
#include "../app.h"
#include "../document.hpp"

// pos in transformed coords
// text origin is at lower left of first character
void display_layout::text_out(pxp const & pos_in,std::string const & text, font* font_in)
{
   assert( font_in && __LINE__);
   if ( text == ""){
      return;
   }

   auto pos = pos_in;
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