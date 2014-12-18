#ifndef OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED
#define OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED

#include <wx/wx.h>
//#include <quan/two_d/vect.hpp>
//#include <quan/three_d/vect.hpp>
//#include <quan/length.hpp>
//#include <quan/reciprocal_length.hpp>
//#include <quan/gx/wxwidgets/graphics_context.hpp>
//#include <quan/gx/simple_drawing_view.hpp>
//#include <quan/gx/simple_device_window.hpp>
//#include "app.h"
//#include "document.hpp"
//#include "drawing.hpp"

   struct bitmap_preview : wxWindow{

      bitmap_preview(wxWindow* parent);

      void OnPaint(wxPaintEvent & event);

      DECLARE_EVENT_TABLE()
   };

#endif // OSD_BITMAP_MAKER_BITMAP_PREVIEW_HPP_INCLUDED
