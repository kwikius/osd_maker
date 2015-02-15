
#include "../../osd_bmp_app.hpp"
#include "../../document.hpp"

#include "../osd_device.hpp"

bool   osd_device::set_clip_rect(osd_device::pxp_type const & minimums, 
                                       osd_device::pxp_type const & maximums)
{
  return false;
}

wxImage const & osd_device::get_bitmap()const
{
   return m_image;
}

void osd_device::rescale(quan::uav::osd::size_type const & new_size)
{
   m_image.Rescale(new_size.x, new_size.y);
}

osd_device::pxp_type osd_device::transform_to_raw(pxp_type const & pos)const
{
  auto ds = get_display_size();
  return {pos.x + ds.x/2 ,ds.y/2 - pos.y};
}

osd_device::pxp_type osd_device::transform_from_raw(pxp_type const & raw_pos)const
{
   auto ds = get_display_size();
   return {raw_pos.x - ds.x/2 , ds.y /2 - raw_pos.y};
}

osd_device::osd_device() : quan::uav::osd::dynamic::display_device()
{
//   if (! m_image.HasAlpha()){
//      m_image.SetAlpha();
//   }
//   if (! m_background_image.HasAlpha()){
//      m_background_image.SetAlpha();
//   }
   if (! wxImage::FindHandler (wxBITMAP_TYPE_PNG)) {
         wxImage::AddHandler (new wxPNGHandler);
   }
   wxImage bkgnd_image{wxT("resources/fpv_background.png")};
   if (!bkgnd_image.IsOk()){
      wxMessageBox(wxT("bk fail"));
   }
   bkgnd_image.Resize(wxSize{384,288},wxPoint{-50,-50});
   m_background_image = bkgnd_image;

   // add Alpha to Bkgnd image?
   m_image = bkgnd_image;
   auto display_size = get_display_size();
   auto left_top = transform_from_raw({0,0});
   auto right_bottom = transform_from_raw(display_size);
   m_display_rect.left = left_top.x;
   m_display_rect.top =  left_top.y;
   m_display_rect.right = right_bottom.x;
   m_display_rect.bottom = right_bottom.y;
   m_clip.set_clipbox(m_display_rect);
}

//void osd_device::set_origin(pxp_type const & p) { m_origin = p;}

osd_device::size_type osd_device::get_display_size() const
{
  return {m_background_image.GetWidth(), m_background_image.GetHeight()};
}

void osd_device::clear()
{
   m_image = m_background_image;
}

/*
 pxp is raw coordinate
*/
void osd_device::set_pixel_raw(pxp_type const & px,colour_type c)
{
   switch (c) {
      case colour_type::black:
         m_image.SetRGB (px.x, px.y, 0, 0, 0);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_OPAQUE);
         break;
      case colour_type::white:
         m_image.SetRGB (px.x, px.y, 255, 255, 255);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_OPAQUE);
         break;
      case colour_type::grey:
         m_image.SetRGB (px.x, px.y, 127, 127, 127);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_OPAQUE);
         break;
      case colour_type::transparent:
         m_image.SetRGB (px.x, px.y, 0, 0, 0);
         m_image.SetAlpha (px.x, px.y, wxIMAGE_ALPHA_TRANSPARENT);
         break;
      default:
        assert(false && __LINE__);
         break;
   }
}


//raw p
osd_device::colour_type osd_device::get_pixel_raw(pxp_type const & p)const
{
#if 0
    auto ch1 = m_image.GetAlpha(p.x,p.y);
    if ( ch1 == wxIMAGE_ALPHA_TRANSPARENT){
     // assert(false);
      return colour::transparent;
    }
#endif
    {
       unsigned int ch =  m_image.GetRed(p.x,p.y);
       switch(ch){
         case 255U:
           return colour_type::white;
          break;
         case 127U:
            return colour_type::grey;
         break;
         case 0U:
            return colour_type::black;
         break;
         default:
         //   assert(false && __LINE__);
            return colour_type::transparent;
       }
   }
}
