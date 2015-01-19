#include "../display_layout.hpp"

/*
 pxp is raw coordinate
*/
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