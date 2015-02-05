#ifndef OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED

#include <string>
#include <quan/two_d/clip.hpp>
#include <quan/two_d/box.hpp>
#include <quan/angle.hpp>
#include <wx/wx.h>
#include <wx/image.h>
#include <quan/two_d/vect.hpp>
#include "osd_bitmap.hpp"
#include "font.hpp"

struct osd_device{
   osd_device();
   typedef quan::two_d::vect<int> pxp;
   typedef quan::two_d::vect<int> size_type;
   typedef quan::two_d::vect<int> pos_type;
   typedef osd_bitmap::colour colour;
   void set_pixel( pxp const & px, colour c);
   void set_pixel_raw(pxp const & px,colour c);
   colour get_pixel_raw(pxp const & px);
   void bitmap(pxp const & pos, osd_bitmap* image);
   void rotated_bitmap_out(pxp const & pos, osd_bitmap* image, pxp const & rotation_centre, quan::angle::deg const & angle);
   void text( pxp const & pos,std::string const & str, font* font_in);
   void line(pxp const & from, pxp const & to,colour c);
 //  void arc_out(pxp const & pos_in,uint32_t  radius, quan::angle::deg const & start_angle,quan::angle::deg const & end_angle, uint32_t numsegs, colour c);
   void arc(pxp const & pos,uint32_t radius,quan::angle::deg const & start_angle,quan::angle::deg const & end_angle,colour c);
   //void circle_out(pxp const & pos_in, int radius, int numsegs, colour c);
   void circle(pxp const & pos_in, int radius,colour c);
   void flood_fill(pxp const & start_pos, colour new_colour);
   size_type get_display_size() const;
   //void set_origin(pos_type const & p);
   void rescale(osd_bitmap::size_type const & new_size);
   pos_type transform_to_raw(pos_type const & pos);
   pos_type transform_from_raw(pos_type const & pos);
   void clear();
  // bool set_background(wxString const & filename);
   wxImage const & get_bitmap()const;
   private:
   wxImage m_background_image;
   wxImage m_image;
   quan::two_d::box<int> m_display_rect;
   quan::two_d::clip<int> m_clip;
   typedef quan::two_d::clip<int>::result_type clip_result_type;
   
};

#endif // OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
