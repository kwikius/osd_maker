#ifndef OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED

#include <string>
#include <quan/two_d/clip.hpp>
#include <quan/two_d/box.hpp>
#include <quan/angle.hpp>
#include <wx/wx.h>
#include <wx/image.h>
#include <quan/uav/osd/dynamic/display_device.hpp>

struct osd_device : quan::uav::osd::dynamic::display_device{
   osd_device();
   void set_pixel_raw(pxp_type const & px,colour_type c);
   colour_type get_pixel_raw(pxp_type const & px) const;
   size_type get_display_size() const;
   pxp_type transform_to_raw(pxp_type const & pos)const;
   pxp_type transform_from_raw(pxp_type const & pos)const;
   bool  set_clip_rect(pxp_type const & minimums, 
                                       pxp_type const & maximums);
   void rescale(size_type const & new_size);
   void clear();
   wxImage const & get_bitmap()const;
   void set_display_buffer(uint32_t offset32,uint32_t mask,colour_type c);
   ~osd_device(){}
   private:
   wxImage m_background_image;
   wxImage m_image;
   quan::two_d::box<int> m_display_rect;
   quan::two_d::clip<int> m_clip;
   typedef quan::two_d::clip<int>::result_type clip_result_type;
  
   
};

#endif // OSD_BITMAP_MAKER_DISPLAY_LAYOUT_HPP_INCLUDED
