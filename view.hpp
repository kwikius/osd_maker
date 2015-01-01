#ifndef AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
#define AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED

#include <wx/wx.h>
#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <quan/reciprocal_length.hpp>
#include <quan/gx/wxwidgets/graphics_context.hpp>
#include <quan/gx/simple_drawing_view.hpp>
#include <quan/gx/simple_device_window.hpp>
#include "app.h"
#include "document.hpp"
#include "drawing.hpp"
#include "osd_image.hpp"

   struct view : wxWindow{

      view(wxWindow* parent);

      void OnPaint(wxPaintEvent & event);
      void OnSize(wxSizeEvent & event);
      void OnScroll(wxScrollWinEvent & event);
      void OnHScroll(wxScrollWinEvent & event);
      void OnVScroll(wxScrollWinEvent & event);
      void OnMouseLeftDown(wxMouseEvent & event);
      void OnMouseLeftUp(wxMouseEvent & event);
      void OnMouseMove(wxMouseEvent & event);
      void OnChar(wxKeyEvent & event);


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
      // copy image refeed by handle to view for display
      void copy_to_current_image(int handle);
      // make a heap copy of current view image
      osd_image* clone_current_image()const
      {
         if ( m_current_image){
            return m_current_image->clone();
         }else{
            return nullptr;
         }
      }
      bool have_image()
      {
            return m_current_image != nullptr;
      }
      int get_doc_image_handle()
      {
         assert ( this->have_image() &&  __LINE__);

         return m_document_image_handle;
      }
      void set_modified(bool val);
      bool is_modified()const { return m_current_image_modified;}

   private:
      void paint_bitmap_view(wxPaintEvent & event);
    // void paint_test_view(wxPaintEvent & event);
      drawing  m_drawing;
      quan::gx::simple_drawing_view m_drawing_view;
      quan::gx::simple_device_window m_device_window;
      vect2_d m_cur_mouse_pos;
      bool m_mouse_is_down;

      DECLARE_EVENT_TABLE()
      
      osd_image* m_current_image;
      int m_document_image_handle;
      bool m_current_image_modified;
      
   };

#endif // AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
