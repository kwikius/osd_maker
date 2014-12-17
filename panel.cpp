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
#include <quan/length.hpp>
#include <quan/reciprocal_length.hpp>

namespace {
quan::two_d::vect<int> vect_mm_to_px(quan::two_d::vect<quan::length::mm> const & in)
{
  wxSize dsmm = wxGetDisplaySizeMM();
  wxSize dspx = wxGetDisplaySize();
  quan::two_d::vect<quan::reciprocal_length::per_mm> 
  mm_to_px{dspx.x/quan::length::mm{dsmm.x},dspx.y/quan::length::mm{dsmm.y}};
  quan::two_d::vect<int> result{
     quan::arithmetic_convert<int>(in.x * mm_to_px.x)
   , quan::arithmetic_convert<int>(in.y * mm_to_px.y)
  };
   return result;
}
}
//return y pos
int panel::make_port_controls(wxBoxSizer* vert_sizer)
{
   quan::two_d::vect<int> const bd = vect_mm_to_px({quan::length::mm{6},quan::length::mm{6}});

   auto sizer = new wxStaticBoxSizer(wxVERTICAL,this,wxT("Serial Port"));

   wxClientDC dc(this);
   wxSize te = dc.GetTextExtent(wxT("-------------------------------"));
   PortText = new wxTextCtrl{this,wxID_ANY, wxT ("/dev/ttyUSB0"),wxDefaultPosition,{te.x + 2 * bd.x,wxDefaultSize.y}};
   sizer->Add(PortText,0,wxLEFT| wxTOP | wxRIGHT | wxBOTTOM,bd.x);

   BtnConnect = new wxButton {this, idBtnConnect, wxT ("Connect")};   
   sizer->Add(BtnConnect,0,wxLEFT| wxRIGHT | wxBOTTOM | wxALIGN_CENTRE,bd.x);

   vert_sizer->Add (sizer,0,wxALL,5);
   return 0;
   
}

int panel::make_scale_controls(wxBoxSizer* vert_sizer, int y_in)
{
   int y = y_in;
   quan::two_d::vect<int> const bd = vect_mm_to_px({quan::length::mm{6},quan::length::mm{6}});

   auto sizer = new wxStaticBoxSizer(wxVERTICAL,this,wxT("Bitmap Scale"));
   wxClientDC dc(this);
   wxSize te = dc.GetTextExtent(wxT("-------------------------------"));
   ScaleText = new wxTextCtrl{this,wxID_ANY,wxT ("1.0"),wxDefaultPosition,{te.x + 2 * bd.x,wxDefaultSize.y}
   ,wxTE_READONLY | wxTE_RIGHT };
   sizer->Add(ScaleText,0,wxLEFT| wxTOP | wxRIGHT | wxBOTTOM,bd.x);

   auto min_scale = 1;
   auto max_scale = 100;
   ScaleSlider = new wxSlider (this,idScaleSlider,100,min_scale,max_scale, wxDefaultPosition,wxSize (200,wxDefaultSize.y),
                               wxSL_HORIZONTAL ,wxDefaultValidator,wxT ("Scale"));
   sizer->Add(ScaleSlider,0,wxLEFT| wxRIGHT | wxBOTTOM,bd.x);

   vert_sizer->Add (sizer,0,wxALL,5);
   return y;
  
}
int panel::make_bitmap_info_controls(wxBoxSizer* vert_sizer,int y_in)
{
   quan::two_d::vect<int> const bd = vect_mm_to_px({quan::length::mm{6},quan::length::mm{6}});

   auto sizer = new wxStaticBoxSizer(wxVERTICAL,this,wxT("Bitmap Info"));
   
   wxClientDC dc(this);

   wxSize te = dc.GetTextExtent(wxT("10000000"));

   {
      auto idx_box = new wxBoxSizer(wxHORIZONTAL);
      auto idxtext = new wxStaticText(this,-1,wxT("Current index"));
      CurrentBitmapIndex = new wxTextCtrl{
         this,wxID_ANY,
         wxT ("~"),
         wxDefaultPosition,wxSize {te.x,wxDefaultSize.GetHeight()}
         ,wxTE_READONLY | wxTE_RIGHT
      };
      idx_box->Add(idxtext,0,wxLEFT| wxTOP ,bd.x);
      idx_box->Add(CurrentBitmapIndex,0,wxLEFT| wxTOP | wxRIGHT ,bd.x);
      sizer->Add(idx_box,0,wxALL,5);
   }
   {
      auto xbox = new wxBoxSizer(wxHORIZONTAL);
      auto xtext = new wxStaticText(this,-1,wxT("num pixels X  "));
      XsizeText = new wxTextCtrl{
         this,wxID_ANY,
         wxT ("~"),
         wxDefaultPosition,wxSize{te.x,wxDefaultSize.GetHeight()}
         ,wxTE_READONLY | wxTE_RIGHT
      };
      xbox->Add(xtext,0,wxLEFT| wxTOP | wxBOTTOM,bd.x);
      xbox->Add(XsizeText,0,wxLEFT| wxTOP | wxRIGHT | wxBOTTOM,bd.x);
      sizer->Add(xbox,0,wxALL,5);
   }
   {
   auto ybox = new wxBoxSizer(wxHORIZONTAL);
   auto ytext = new wxStaticText(this,-1,wxT("num pixels Y  "));
   YsizeText=new wxTextCtrl{
      this,wxID_ANY,
      wxT ("~"),
      wxDefaultPosition,wxSize {te.x,wxDefaultSize.GetHeight()}
      ,wxTE_READONLY | wxTE_RIGHT
   };
   ybox->Add(ytext,0,wxLEFT| wxBOTTOM,bd.x);
   ybox->Add(YsizeText,0, wxLEFT| wxRIGHT | wxBOTTOM,bd.x);
   sizer->Add(ybox,0,wxALL,5);
   vert_sizer->Add (sizer,0,wxALL,5);
   } 
   return y_in;
}

panel::panel (wxWindow* parent)
:  wxScrolledWindow {parent}
,BtnConnect {nullptr}
,PortText {nullptr}
 ,ScaleText{nullptr}
  ,ScaleSlider{nullptr}
,CurrentBitmapIndex{nullptr}
 ,XsizeText{nullptr}
,YsizeText{nullptr}
{
   // add scrolling
   window_ids::panel = this->GetId();
   SetBackgroundColour(*wxLIGHT_GREY);
   //Refresh();
   
   auto vert_sizer = new wxBoxSizer (wxVERTICAL);
  
   int y = make_port_controls(vert_sizer);
   y = make_scale_controls(vert_sizer,y);
   make_bitmap_info_controls(vert_sizer,y);
  // auto & app = wxGetApp();
 //  auto doc = app.get_document();

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
   EVT_COMMAND_SCROLL (idScaleSlider,panel::OnScaleSlider)
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
         app.m_sp_CS.Leave();
         testsp =0;
         BtnConnect->SetLabel (wxT ("Disconnect"));
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
      BtnConnect->SetLabel (wxT ("Connect"));
   }
}


