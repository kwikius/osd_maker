#ifndef AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
#define AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED

#include <wx/wx.h>
#include <wx/dynlib.h>
#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <quan/reciprocal_length.hpp>
#include <quan/angle.hpp>
#include <quan/gx/wxwidgets/graphics_context.hpp>
#include <quan/gx/simple_drawing_view.hpp>
#include <quan/gx/simple_device_window.hpp>
#include <quan/uav/osd/dynamic/bitmap.hpp>
#include <quan/uav/osd/dynamic/font.hpp>
#include <quan/uav/osd/dynamic/object_database.hpp>
#include "../osd_bmp_app.hpp"
#include "../document.hpp"
#include "view/drawing.hpp"
#include "../graphics_api/osd_device.hpp"

extern "C"{
     typedef  void (*pfn_set_osd_on_draw_params) ( quan::uav::osd::dynamic::object_database const * db);

      typedef  void (*pfn_osd_on_draw)(quan::uav::osd::dynamic::display_device & d);
}

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

      typedef quan::uav::osd::dynamic::bitmap dynamic_bitmap;
      typedef quan::uav::osd::dynamic::font dynamic_font;

      bool get_image_pixel(vect2_d const & event_pos, quan::uav::osd::pxp_type & result_pos);
      quan::gx::abc_color::ptr get_colour(quan::uav::osd::colour_type colour_id);
      // returns result of yes no cancel messgaebox
      int sync_hmi_view();
      void sync_to_document();
      //see panel and
      bool sync_with_image_handle(int handle);
      // copy image referred by handle to view for display
      void copy_to_current_image(int handle);
      // make a heap copy of current view image
      dynamic_bitmap* clone_current_image()const
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
      bool resize_image( quan::two_d::box<int> new_size);
      void set_modified(bool val);
      bool is_modified()const { return m_current_image_modified;}
      enum class view_mode {Unknown, inBitmaps, inLayouts};
      view_mode get_view_mode() const{ return m_view_mode;}
      void set_view_mode(view_mode mode) ;//{m_view_mode = mode;}
      dynamic_font* get_current_font()const;
    //  quan::angle::deg get_bearing()const { return m_bearing;}
     // void set_bearing( quan::angle::deg const & angle){m_bearing = angle;}
     // quan::angle::deg get_home_bearing()const { return m_home_bearing;}
     // void set_home_bearing( quan::angle::deg const & angle){m_home_bearing = angle;}
      bool Destroy();
   private:
      void OnPaint(wxPaintEvent & event);
      void OnSize(wxSizeEvent & event);
      void OnScroll(wxScrollWinEvent & event);
      void OnHScroll(wxScrollWinEvent & event);
      void OnVScroll(wxScrollWinEvent & event);
      void OnMouseLeftDown(wxMouseEvent & event);
      void OnMouseRightDown(wxMouseEvent & event);
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

      void setup_draw_fn();

      // a clone of the bitmap whose handle is in 
      // m_document_image_handle
      dynamic_bitmap* m_current_image;
      // handle of the bitmap that the view represents
      int m_document_image_handle;
     // int m_last_font_handle;
      bool m_current_image_modified;
      view_mode m_view_mode;

      osd_device m_osd_device;

      pfn_set_osd_on_draw_params m_pfn_set_osd_on_draw_params;
      pfn_osd_on_draw m_pfn_osd_on_draw;

      wxDynamicLibrary m_dll;

   };

#endif // AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
