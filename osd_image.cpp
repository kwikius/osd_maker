#include <wx/bitmap.h>
#include <wx/rawbmp.h>

#include "osd_image.hpp"

osd_bitmap* ConvertTo_osd_bitmap (std::string const & name, wxImage const& image)
{
   osd_image::size_type bitmap_size {image.GetWidth(), image.GetHeight() };
   osd_bitmap * bmp = new osd_bitmap {name, bitmap_size};

   for (uint32_t y = 0; y < bitmap_size.y; ++y) {
         for (uint32_t x = 0; x < bitmap_size.x; ++x) {
               osd_image::colour  colour = osd_image::colour::transparent;
               if (!image.IsTransparent (x, y)) {
                     uint8_t red = image.GetRed (x, y);
                     uint8_t green = image.GetGreen (x, y);
                     uint8_t blue = image.GetBlue (x, y);
                     float mono = (red * 0.2126 + green * 0.7152 + blue * 0.0722) / 255;
                     if (mono < 1. / 4) {
                           colour = osd_image::colour::black;
                        }
                     else {
                           if (mono < 1. / 2) {
                                 colour = osd_image::colour::grey;
                              }
                           else {
                                 colour = osd_image::colour::white;
                              }
                        }
                  }
               bmp->set_pixel_colour ( {x, y}, colour);
            }
      }
   return bmp;
}


// returns ptr to a new bitmap created on heap with new
// transparent colour
// and scale
// the array of colours offsets
//  grey = 0b00,
//    black = 0b01,
//    white = 0b10,
// transparent = 0b11
wxBitmap* ConvertTo_wxBitmap (osd_image const& in, wxColour const* (&colours) [4])
{
   osd_image::size_type size = in.get_size();
   auto bmp = new wxBitmap (size.x, size.y);
   assert (bmp && __LINE__);
   wxNativePixelData pixels (*bmp);
   for (size_t y = 0; y < size.y; ++y) {
         for (size_t x = 0; x < size.x; ++x) {
               osd_image::colour c;
               in.get_pixel_colour ( {x, y}, c);
               assert ( (c != osd_image::colour::invalid) && __LINE__);
               auto const idx = static_cast<uint32_t> (c);
               assert ( (idx < 4) && __LINE__);
               wxColour const * colour = colours[idx];
               wxNativePixelData::Iterator p (pixels);
               p.MoveTo (pixels, x, y);
               p.Red() = colour->Red();
               p.Green() = colour->Green();
               p.Blue() = colour->Blue();
               // p.Alpa() = colour->Alpha();
            }
      }
   return bmp;
}
//  grey = 0b00,
//    black = 0b01,
//    white = 0b10,
// transparent = 0b11
 
// caller owns the reulting imag
//call delete to free
wxImage* ConvertTo_wxImage (osd_image const& in)
{
   osd_image::size_type size = in.get_size();
   auto img = new wxImage (size.x, size.y);
   assert (img && __LINE__);
   if (!img->HasAlpha()) {
         img->SetAlpha();
      }
   for (size_t y = 0; y < size.y; ++y) {
         for (size_t x = 0; x < size.x; ++x) {
               osd_image::colour c;
               in.get_pixel_colour ( {x, y}, c);
               switch (c) {
                     case osd_image::colour::black:
                        img->SetRGB (x, y, 0, 0, 0);
                        img->SetAlpha (x, y, wxIMAGE_ALPHA_OPAQUE);
                        break;
                     case osd_image::colour::white:
                        img->SetRGB (x, y, 255, 255, 255);
                        img->SetAlpha (x, y, wxIMAGE_ALPHA_OPAQUE);
                        break;
                     case osd_image::colour::grey:
                        img->SetRGB (x, y, 127, 127, 127);
                        img->SetAlpha (x, y, wxIMAGE_ALPHA_OPAQUE);
                        break;
                     default:
                        img->SetRGB (x, y, 0, 0, 0);
                        img->SetAlpha (x, y, wxIMAGE_ALPHA_TRANSPARENT);
                        break;
                  }
            }
      }
   return img;
   
}
 
bool osd_bitmap::get_pixel_colour (pos_type const & p, osd_image::colour & c) const
{
   if ( (p.x >= m_size.x) || (p.y >= m_size.y)) {
         return false;
      }
   auto idx = p.y * m_size.x + p.x;
   c = m_data.at (idx);
   return true;
}
bool osd_bitmap::set_pixel_colour (pos_type const & p, osd_image::colour c)
{
   if (c == colour::invalid) {
         return false;
      }
   if ( (p.x >= m_size.x) || (p.y >= m_size.y)) {
         return false;
      }
   auto idx = p.y * m_size.x + p.x;
   m_data.at (idx) = c;
   return true;
}
 