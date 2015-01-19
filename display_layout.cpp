#include "app.h"
#include "display_layout.hpp"
#include "document.hpp"

wxImage const & display_layout::get_image()const
{
   return m_image;
}

void display_layout::rescale(osd_image::size_type const & new_size)
{
   m_image.Rescale(new_size.x, new_size.y);
}

display_layout::pos_type display_layout::transform_to_raw(pos_type const & pos)
{
  auto ds = get_display_size();
  return {pos.x + ds.x/2 ,ds.y/2 - pos.y};
}

display_layout::pos_type display_layout::transform_from_raw(pos_type const & raw_pos)
{
   auto ds = get_display_size();
   return {raw_pos.x - ds.x/2 , ds.y /2 - raw_pos.y};
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
   auto display_size = get_display_size();
   auto left_top = transform_from_raw({0,0});
   auto right_bottom = transform_from_raw(display_size);
   m_display_rect.left = left_top.x;
   m_display_rect.top =  left_top.y;
   m_display_rect.right = right_bottom.x;
   m_display_rect.bottom = right_bottom.y;
   m_clip.set_clipbox(m_display_rect);
};

//void display_layout::set_origin(pos_type const & p) { m_origin = p;}

display_layout::size_type display_layout::get_display_size() const
{
  return {m_background_image.GetWidth(), m_background_image.GetHeight()};
}

void display_layout::clear()
{
   m_image = m_background_image;
}
