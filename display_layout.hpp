#ifndef OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED

#include <string>

#include <wx/wx.h>
#include <wx/image.h>
#include <quan/two_d/vect.hpp>
#include "osd_image.hpp"
#include "font.hpp"


/*
   layout_item
    text( font, pos, string)
    bitmap()
    value
*/

struct display_layout{
   display_layout();
   typedef quan::two_d::vect<int> pxp;
   typedef osd_image::colour colour;
   void set_pixel(pxp const & px,colour c);
   void bitmap_out(pxp const & pos, osd_image* image);
   void text_out( pxp const & pos,std::string const & str, font* font_in);
   void clear();
  // bool set_background(wxString const & filename);
   wxImage const & get_image()const;
   private:
   wxImage m_background_image;
   wxImage m_image;
   
};
/*
variables
  pitch
  roll
  yaw
*/

#endif // OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
