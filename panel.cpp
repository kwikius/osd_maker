#include "panel.hpp"
#include <wx/wx.h>
#include <quan/serial_port.hpp>
#include "app.h"
#include <quan/gx/wxwidgets/from_wxString.hpp>
#include "window_ids.hpp"
#include "events.hpp"
#include "document.hpp"
#include "main_frame.h"
#include "view.hpp"

panel::panel (wxWindow* parent)
:  wxScrolledWindow {parent}
,BtnConnect {nullptr}
,PortText {nullptr}

{
   // add scrolling
   window_ids::panel = this->GetId();
   
   auto vert_sizer = new wxBoxSizer (wxVERTICAL);
   
   auto sp_sizer =  new wxFlexGridSizer (2,2,4,4);
   auto port_label = new wxStaticText{this,wxID_ANY, wxT ("serial port"),wxDefaultPosition, wxDefaultSize, 0};
   sp_sizer->Add (port_label,0,wxALL,5);
   PortText = new wxTextCtrl{this,wxID_ANY, wxT ("/dev/ttyUSB0"), wxDefaultPosition,wxSize (200,wxDefaultSize.GetHeight()) };
   sp_sizer->Add (PortText,0,wxALL,5);

   BtnConnect = new wxButton (this, idBtnConnect, wxT ("Connect"), wxDefaultPosition, wxSize (100,wxDefaultSize.GetHeight()), 0);
   sp_sizer->Add (BtnConnect,0,wxALL,5);
   vert_sizer->Add (sp_sizer);
   
   auto vert_spacer1 =  new wxPanel{this,0,0, 1, 1, 0,  wxT ("") };
   vert_sizer->Add (vert_spacer1,0,wxALL,10);
  
   auto scale_sizer = new wxFlexGridSizer (2,2,4,4);
   auto scale_spacer =  new wxPanel{this,0,0, 1, 1, 0,  wxT ("") };
   scale_sizer->Add (scale_spacer,0,wxALL,5);
   
   ScaleText =new wxTextCtrl{
      this,wxID_ANY,
      wxT ("1.0"),
      wxDefaultPosition,wxSize (100,wxDefaultSize.GetHeight())
   };

   auto min_scale = 1;
   auto max_scale = 100;
   ScaleSlider = new wxSlider (this,idScaleSlider,100,min_scale,max_scale, wxDefaultPosition,wxSize (200,wxDefaultSize.y),
                               wxSL_HORIZONTAL ,wxDefaultValidator,wxT ("Scale"));
                               
   scale_sizer->Add (ScaleText,0,wxALL,5);
   
   auto scale_label = new wxStaticText{this,wxID_ANY, wxT ("map scale"),wxDefaultPosition, wxDefaultSize, 0};
   scale_sizer->Add (scale_label,0,wxALL,5);
   
   scale_sizer->Add (ScaleSlider,0,wxALL,5);
   vert_sizer->Add (scale_sizer);
   
   auto & app = wxGetApp();
   auto doc = app.get_document();

   auto horz_sizer = new wxBoxSizer (wxHORIZONTAL);

   horz_sizer->Add (vert_sizer);
   this->SetSizer (horz_sizer);
   this->Layout();
   horz_sizer->Fit (this);
   
   // this makes the scrollbars show up
   this->FitInside(); // ask the sizer about the needed size
   this->SetScrollRate (5, 5);
}

BEGIN_EVENT_TABLE (panel, wxPanel)
   EVT_BUTTON (idBtnConnect, panel::OnConnectDisconnect)
  // EVT_CUSTOM (wxEvent_AircraftPositionChanged,wxID_ANY,panel::OnAircraftPositionChanged)
  // EVT_CUSTOM (wxEvent_RemoteDistanceChanged,wxID_ANY,panel::OnRemoteDistanceChanged)
  // EVT_CUSTOM (wxEvent_RemoteBearingChanged,wxID_ANY,panel::OnRemoteBearingChanged)
  // EVT_CUSTOM (wxEvent_RemoteElevationChanged,wxID_ANY,panel::OnRemoteElevationChanged)
 //  EVT_COMMAND_SCROLL (idAltitudeSlider,panel::OnAltitudeSlider)
   EVT_COMMAND_SCROLL (idScaleSlider,panel::OnScaleSlider)
 //  EVT_RADIOBOX (idProtocolChooser,panel::OnProtocolChanged)
END_EVENT_TABLE()

void panel::OnScaleSlider (wxScrollEvent & event)
{
   auto v = ScaleSlider->GetValue();
   double scale =  v / 100.0;
   wxGetApp().get_view()->set_scale (scale);
   ScaleText->ChangeValue (wxString::Format (wxT ("%f"),scale));
}

void panel::OnConnectDisconnect (wxCommandEvent &event)
{
   auto & app = wxGetApp();
   app.m_sp_CS.Enter();
   if (!app.have_sp())
   {
      app.m_sp_CS.Leave();
      std::string text = quan::gx::wxwidgets::from_wxString<char> (PortText->GetValue());
      // do a test open of the port
      auto testsp = new quan::serial_port (text.c_str());
      testsp->init();
      if (testsp->good()) {
         app.m_sp_CS.Enter();
         wxGetApp().set_sp (testsp);
//         OnProtocolChanged();
         app.m_sp_CS.Leave();
         testsp =0;
         BtnConnect->SetLabel (wxT ("Disconnect"));
         // grey ProtocolChooser
    //     ProtocolChooser->Enable(0,false);
     //    ProtocolChooser->Enable(1,false);
         return;
      }
      else {
         delete testsp;
         wxString msg = wxT ("Open \"") + wxString::FromAscii (text.c_str()) + wxT ("\" failed. Check port.");
         wxMessageBox (msg,wxT ("Connect Error"));
         return;
      }
   } else { // want disconnect
      {
         app.close_sp();
         app.m_sp_CS.Leave();
      }
      // ungrey ProtocolChooser
    //  ProtocolChooser->Enable(0,true);
    //  ProtocolChooser->Enable(1,true);
      BtnConnect->SetLabel (wxT ("Connect"));
   }
}


