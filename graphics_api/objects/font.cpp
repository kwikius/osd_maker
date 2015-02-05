#include  "../font.hpp"
#include "../../osd_bmp_app.hpp"
#include "../../document.hpp"

font::bitmap_ptr  font::get_char_at(int32_t i)const 
{
   int char_handle =-1;
   if ( !this->get_handle_at(i,char_handle)){
      return nullptr;
   }
   return wxGetApp().get_document()->get_bitmap(char_handle);
}



