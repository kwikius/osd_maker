#ifndef OSD_BITMAP_MAKER_DB_BITMAP_HPP_INCLUDED
#define OSD_BITMAP_MAKER_DB_BITMAP_HPP_INCLUDED

#include <quan/uav/osd/dynamic/bitmap.hpp>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/rawbmp.h>

wxBitmap* ConvertTo_wxBitmap(quan::uav::osd::dynamic::bitmap const& in,
 wxColour const * (&colours)[4]);
wxImage* ConvertTo_wxImage(quan::uav::osd::dynamic::bitmap const& in);
quan::uav::osd::dynamic::bitmap* 
ConvertTo_osd_bitmap(std::string const & name,wxImage const& image);


#endif // OSD_BITMAP_MAKER_DB_BITMAP_HPP_INCLUDED
