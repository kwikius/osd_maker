#ifndef QUANTRACKER_SIM_PANEL_HPP_INCLUDED
#define QUANTRACKER_SIM_PANEL_HPP_INCLUDED


//#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>

#include "osd_image.hpp"

struct view;
struct panel : wxScrolledWindow// wxPanel
{
  panel (wxWindow * parent); 


private:
  // void set_current_bitmap(osd_image const * image ,uint32_t index);
  // void set_current_bitmap_index(int index);
   enum
   {
      idMenuQuit = 1000,
      idMenuAbout,
      idTimer,
      idBtnConnect,
      idScaleSlider
   };

   DECLARE_EVENT_TABLE()

   void OnConnectDisconnect(wxCommandEvent& event);
 //  void OnAircraftPositionChanged(wxEvent& event);
  // void OnScaleSlider(wxScrollEvent & event);
  // void OnProtocolChanged(wxCommandEvent& event);
   wxButton* BtnConnect;
   wxTextCtrl* PortText;
//scaling of the view
  // wxTextCtrl* ScaleText;
  // wxSlider* ScaleSlider;  

   wxTextCtrl* CurrentBitmapIndex;
   wxTextCtrl* XsizeText;
   wxTextCtrl* YsizeText;

  // wxRadioBox* ProtocolChooser;
   void make_port_controls(wxBoxSizer* s);
  // void make_scale_controls(wxBoxSizer* s);
   void make_bitmap_info_controls(wxBoxSizer* s);

   friend struct view;
};


#endif // QUANTRACKER_SIM_PANEL_HPP_INCLUDED
