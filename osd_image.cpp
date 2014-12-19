
#include <wx/bitmap.h>
#include <wx/rawbmp.h>

#include "osd_image.hpp"
  // returns ptr to a new bitmap created on heap
   // transparent colour
  // and scale
  // the array of colours offsets
  //  grey = 0b00,
  //    black = 0b01,
  //    white = 0b10,
  // transparent = 0b11
  wxBitmap* ConvertTo_wxBitmap(osd_image const& in, wxColour const* (&colours)[4])
  {
     osd_image::size_type size = in.get_size();
     auto bmp = new wxBitmap(size.x,size.y);
     assert(bmp && __LINE__);
     wxNativePixelData pixels(*bmp);
     for ( size_t y = 0; y < size.y; ++y){
         for ( size_t x = 0; x < size.x; ++x){
            osd_image::colour c;
            in.get_pixel_colour({x,y},c);
            assert ( (c != osd_image::colour::invalid) && __LINE__);
            auto const idx = static_cast<uint32_t>(c);
            assert((idx < 4) && __LINE__);
            wxColour const * colour = colours[idx];
            wxNativePixelData::Iterator p(pixels);
            p.MoveTo(pixels,x,y);
            p.Red() = colour->Red();
            p.Green() = colour->Green();
            p.Blue() = colour->Blue();
           // p.Alpa() = colour->Alpha();
         }
      }
      return bmp;
  }