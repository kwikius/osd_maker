
#include "db_bitmap.hpp"


quan::uav::osd::dynamic::bitmap* 
ConvertTo_osd_bitmap (std::string const & name, wxImage const& image)
{
   typedef quan::uav::osd::dynamic::bitmap bitmap;
   typedef quan::uav::osd::size_type size_type; 
   typedef quan::uav::osd::colour_type colour_type;

   size_type bitmap_size {image.GetWidth(), image.GetHeight() };
   bitmap * bmp = new bitmap {name, bitmap_size};
   
   for (int32_t y = 0; y < bitmap_size.y; ++y) {
         for (int32_t x = 0; x < bitmap_size.x; ++x) {
               colour_type  colour = colour_type::transparent;
               if (!image.IsTransparent (x, y)) {
                     uint8_t red = image.GetRed (x, y);
                     uint8_t green = image.GetGreen (x, y);
                     uint8_t blue = image.GetBlue (x, y);
                     float mono = (red * 0.2126 + green * 0.7152 + blue * 0.0722) / 255;
                     if (mono < 1. / 4) {
                           colour = colour_type::black;
                        }
                     else {
                           if (mono < 1. / 2) {
                                 colour = colour_type::grey;
                              }
                           else {
                                 colour = colour_type::white;
                              }
                        }
                  }
               bmp->set_pixel({x,y},colour);
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
wxBitmap* ConvertTo_wxBitmap (quan::uav::osd::dynamic::bitmap const& in, wxColour const* (&colours) [4])
{
   typedef quan::uav::osd::dynamic::bitmap bitmap;
   typedef quan::uav::osd::size_type size_type; 
   typedef quan::uav::osd::colour_type colour_type;

   size_type size = in.get_size();
   auto bmp = new wxBitmap (size.x, size.y);
   assert (bmp && __LINE__);
   wxNativePixelData pixels (*bmp);
   for (int32_t y = 0; y < size.y; ++y) {
      for (int32_t x = 0; x < size.x; ++x) {
         colour_type c =in.get_pixel_colour ({x, y});
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
wxImage* ConvertTo_wxImage (quan::uav::osd::dynamic::bitmap const& in)
{
   //typedef quan::uav::osd::dynamic::bitmap bitmap;
   typedef quan::uav::osd::size_type size_type; 
   typedef quan::uav::osd::colour_type colour_type;

   size_type size = in.get_size();
   auto img = new wxImage (size.x, size.y);
   assert (img && __LINE__);
   if (!img->HasAlpha()) {
         img->SetAlpha();
      }
   for (int32_t y = 0; y < size.y; ++y) {
         for (int32_t x = 0; x < size.x; ++x) {
               colour_type c =in.get_pixel_colour ({x, y});
               switch (c) {
                     case colour_type::black:
                        img->SetRGB (x, y, 0, 0, 0);
                        img->SetAlpha (x, y, wxIMAGE_ALPHA_OPAQUE);
                        break;
                     case colour_type::white:
                        img->SetRGB (x, y, 255, 255, 255);
                        img->SetAlpha (x, y, wxIMAGE_ALPHA_OPAQUE);
                        break;
                     case colour_type::grey:
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