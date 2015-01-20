#ifndef AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
#define AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED

#include <wx/wx.h>
#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <quan/reciprocal_length.hpp>
#include <quan/angle.hpp>
#include <quan/gx/wxwidgets/graphics_context.hpp>
#include <quan/gx/simple_drawing_view.hpp>
#include <quan/gx/simple_device_window.hpp>
#include "app.h"
#include "document.hpp"
#include "drawing.hpp"
#include "osd_image.hpp"

#include "display_layout.hpp"

   struct view : wxWindow{
      view(wxWindow* parent);
     
      void reset();
      document* get_document(){ return wxGetApp().get_document();}
      void set_scale(double const & v);
      double get_scale();
      typedef quan::two_d::vect<int>               vect2_i;
      typedef quan::two_d::vect<uint32_t>          vect2_u;
      typedef quan::two_d::vect<double>            vect2_d;
      typedef quan::length::mm                          mm;
      typedef quan::two_d::vect<mm>               vect2_mm;
      typedef quan::reciprocal_length::per_mm       per_mm;
      typedef quan::two_d::vect<per_mm>       vect2_per_mm;
      typedef quan::three_d::vect<mm>             vect3_mm;

      bool get_image_pixel(vect2_d const & event_pos, osd_image::pos_type & result_pos);
      quan::gx::abc_color::ptr get_colour(osd_image::colour colour_id);
      // returns result of yes no cancel messgaebox
      int sync_hmi_view();
      void sync_to_document();
      //see panel and
      bool sync_with_image_handle(int handle);
      // copy image refeed by handle to view for display
      void copy_to_current_image(int handle);
      // make a heap copy of current view image
      osd_image* clone_current_image()const
      {
         assert(have_image()&& __LINE__);
         return m_current_image->clone();
      }
      bool have_image()const
      {
            return m_current_image != nullptr;
      }
      int get_doc_image_handle()const
      {
         assert ( this->have_image() &&  __LINE__);

         return m_document_image_handle;
      }
      void set_modified(bool val);
      bool is_modified()const { return m_current_image_modified;}
      enum class view_mode {Unknown, inBitmaps, inLayouts};
      view_mode get_view_mode() const{ return m_view_mode;}
      void set_view_mode(view_mode mode) ;//{m_view_mode = mode;}
      font* get_current_font()const;
      quan::angle::deg get_bearing()const { return m_bearing;}
      void set_bearing( quan::angle::deg const & angle){m_bearing = angle;}
      quan::angle::deg get_home_bearing()const { return m_home_bearing;}
      void set_home_bearing( quan::angle::deg const & angle){m_home_bearing = angle;}
   private:
      void OnPaint(wxPaintEvent & event);
      void OnSize(wxSizeEvent & event);
      void OnScroll(wxScrollWinEvent & event);
      void OnHScroll(wxScrollWinEvent & event);
      void OnVScroll(wxScrollWinEvent & event);
      void OnMouseLeftDown(wxMouseEvent & event);
      void OnMouseLeftUp(wxMouseEvent & event);
      void OnMouseMove(wxMouseEvent & event);
      void OnChar(wxKeyEvent & event);
      void paint_bitmap_view(wxPaintEvent & event);
      void paint_layout_view(wxPaintEvent & event);
      drawing  m_drawing;
      quan::gx::simple_drawing_view m_drawing_view;
      quan::gx::simple_device_window m_device_window;
      void on_bitmaps_char(wxKeyEvent & event);
      vect2_d m_cur_mouse_pos;
      bool m_mouse_is_down;

      DECLARE_EVENT_TABLE()
      
      osd_image* m_current_image;
      int m_document_image_handle;
     // int m_last_font_handle;
      bool m_current_image_modified;
      view_mode m_view_mode;
      
      display_layout m_display_layout;

      quan::angle::deg m_bearing;
      quan::angle::deg m_home_bearing;
      
   };

#endif // AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
