/***************************************************************
 * Name:      main_frame.cpp
 * Purpose:   Code for Application Frame
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2013-03-06
 * Copyright: Andy (http://www.softpainter.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "main_frame.h"

#include "document.hpp"
#include "sp_in_thread.hpp"

#include "app.h"
#include "splitter.hpp"
#include "sp_in_thread.hpp"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(main_frame, wxFrame)
    EVT_CLOSE(main_frame::OnClose)
    EVT_MENU(idMenuQuit, main_frame::OnQuit)
    EVT_MENU(idMenuAbout, main_frame::OnAbout)
    EVT_MENU(wxID_OPEN,main_frame::OnFileOpen)
    EVT_TIMER(idTimer, main_frame::OnTimer)
END_EVENT_TABLE()

main_frame::main_frame(wxFrame *frame, const wxString& title, wxSize const & size)
    : wxFrame(frame, -1, title, wxDefaultPosition, size )
     , m_splitter{nullptr}
      ,Timer{nullptr}
      ,m_sp_in_thread{nullptr}
{
     
    this->m_splitter = new splitter(this);
    create_menus();
    create_statusbar();

//#endif // wxUSE_STATUSBAR

  //  Timer= new wxTimer{this,idTimer};
    // update rate of 1/5th sec
  //  Timer->Start(200,wxTIMER_CONTINUOUS); 

   // m_sp_in_thread = new sp_in_thread(this);
  //  m_sp_in_thread->Create();
  //  m_sp_in_thread->Run();

}

main_frame::~main_frame()
{}

void main_frame::create_menus()
{
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(wxID_OPEN, _("&Open\tCtrl+O"), _("Open File"));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
}
void main_frame::create_statusbar()
{
    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to OSD BitmapMaker"),0);
    SetStatusText(wxT("V1.0"), 1);
}

bool main_frame::Destroy()
{
   {
      wxCriticalSectionLocker lock(m_thread_CS);
      if ( m_sp_in_thread != nullptr){
         m_sp_in_thread->Delete();
      };
   }

   for (;;)
   {
      wxCriticalSectionLocker lock(m_thread_CS);
      if ( m_sp_in_thread == nullptr){
         break;
      }
   }
   return wxFrame::Destroy();
}

void main_frame::OnClose(wxCloseEvent &event)
{


    Destroy();
}

void main_frame::OnQuit(wxCommandEvent &event)
{
    wxSize frame_size = this->GetSize();
    auto config = wxGetApp().get_config();
    config->Write(wxT("/MainFrame/InitialWidth"),frame_size.x);
    config->Write(wxT("/MainFrame/InitialHeight"),frame_size.y);

    Destroy();
}

void main_frame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void main_frame::OnFileOpen(wxCommandEvent &event)
{

  wxFileDialog fd{
   this,
   wxT("Open File"),  // message
   wxT(""),                     // default dir
   wxT(""),                     // default file
   wxT("*.mcm;*.png"),          // wildcard
   wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR,
   wxDefaultPosition,
   wxDefaultSize,
   wxT("Open File Dialog")
 };

  if ( (fd.ShowModal() == wxID_OK)  ){
/*
      wxString str = fd->GetPath();
      wchar_t const * cp = str.c_str();
      std::wstring str2(cp);   
      std::string std_str = quan::to_string<char>(str2);
      std::ofstream file("output_name.txt");
      file << "#" << std_str << "#\n";
     // std::ostringstream ostr;
      if( this->m_view->m_doc->m_wing_templates.size() ==0){
        this->m_view->m_doc->m_wing_templates.push_back(wing_template{});
      }
      if( this->m_view->m_doc->m_wing_templates[0].m_aerofoil.load(std_str,file)){
         this->m_view->m_doc->set_modified(true);
         SetStatusText(quan::gx::wxwidgets::to_wxString(this->m_view->m_doc->m_wing_templates[0].m_aerofoil.get_name()),1);
      }else{
        wxMessageBox(wxT("file load failed\n"));
      }
*/
   }
  // fd->Destroy();
  // if (this->m_view->m_doc->is_modified()){
      
    //  this->m_view->Refresh();
   //}

}



/*
 Timer represents the update rate of the  airborne telemetry unit
*/
void main_frame::OnTimer(wxTimerEvent &event)
{

}
