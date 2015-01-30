#include "osd_device.hpp"

/*
 pxp is raw coordinate
*/
void osd_device::set_pixel_raw(pxp const & px,colour c)
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
        assert(false && __LINE__);
         break;
   }
}

// transformed
void osd_device::set_pixel( pxp const & px, colour c)
{
   set_pixel_raw(transform_to_raw(px),c);
}

//raw p
osd_device::colour osd_device::get_pixel_raw(pxp const & p)
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
           return colour::white;
          break;
         case 127U:
            return colour::grey;
         break;
         case 0U:
            return colour::black;
         break;
         default:
         //   assert(false && __LINE__);
            return colour::invalid;
       }
   }
}