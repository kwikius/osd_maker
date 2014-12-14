#ifndef AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
#define AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED

#include <wx/wx.h>

#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <quan/reciprocal_length.hpp>

#include <quan/gx/wxwidgets/graphics_context.hpp>
//#include <quan/gx/simple_drawing.hpp>
#include <quan/gx/simple_drawing_view.hpp>
#include <quan/gx/simple_device_window.hpp>
#include "app.h"
#include "drawing.hpp"

//#include "aerofoilDoc.hpp"

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
           
      typedef quan::two_d::vect<int>               vect2_i;
      typedef quan::two_d::vect<uint32_t>          vect2_u;
      typedef quan::two_d::vect<double>            vect2_d;
      typedef quan::length::mm                          mm;
      typedef quan::two_d::vect<mm>               vect2_mm;
      typedef quan::reciprocal_length::per_mm       per_mm;
      typedef quan::two_d::vect<per_mm>       vect2_per_mm;
      typedef quan::three_d::vect<mm>             vect3_mm;

      bool get_image_pixel(vect2_d const & event_pos, vect2_i & result_pos);
      quan::gx::abc_color::ptr get_colour(uint8_t colour_id);
      
   private:


    //  quan::gx::simple_drawing<mm> m_drawing ;
      drawing  m_drawing;
      quan::gx::simple_drawing_view m_drawing_view;
      quan::gx::simple_device_window m_device_window;
      
      bool  m_aircraft_selected;
      vect3_mm  m_last_aircraft_position;
      DECLARE_EVENT_TABLE()
      vect2_d m_cur_mouse_pos;
   };


#endif // AEROFOIL_GRAPHICS_WINDOW_HPP_INCLUDED
