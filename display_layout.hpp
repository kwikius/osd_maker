#ifndef OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED

#include <string>
#include <quan/two_d/clip.hpp>
#include <quan/two_d/box.hpp>
#include <wx/wx.h>
#include <wx/image.h>
#include <quan/two_d/vect.hpp>
#include "osd_image.hpp"
#include "font.hpp"

struct display_layout{
   display_layout();
   typedef quan::two_d::vect<int> pxp;
   typedef quan::two_d::vect<int> size_type;
   typedef osd_image::colour colour;
   void set_pixel(pxp const & px,colour c);
   void bitmap_out(pxp const & pos, osd_image* image);
   void text_out( pxp const & pos,std::string const & str, font* font_in);
   void line_out(pxp const & from, pxp const & to,colour c);
   size_type get_display_size() const;
   void clear();
  // bool set_background(wxString const & filename);
   wxImage const & get_image()const;
   private:
   wxImage m_background_image;
   wxImage m_image;
   quan::two_d::box<int> m_display_rect;
   quan::two_d::clip<int> m_clip;
   typedef quan::two_d::clip<int>::result_type clip_result_type;
   
};
/*
variables
  pitch
  roll
  yaw
*/

#endif // OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
