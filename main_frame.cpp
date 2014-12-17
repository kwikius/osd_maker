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
     short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
     wxString wxbuild(wxVERSION_STRING);

     if (format == long_f ) {
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
     , m_splitter {nullptr}
,Timer {nullptr}
,m_sp_in_thread {nullptr} {

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
          if ( m_sp_in_thread != nullptr) {
               m_sp_in_thread->Delete();
          };
     }

     for (;;) {
          wxCriticalSectionLocker lock(m_thread_CS);
          if ( m_sp_in_thread == nullptr) {
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
     wxFileDialog fd {
          this,
          wxT("Open File"),  // message
          wxT(""),                     // default dir
          wxT(""),                     // default file
          wxT("PNG files(*.png)|*.png|MinimOSD Charset(*.mcm)|*.mcm"),          // wildcard
          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR,
          wxDefaultPosition,
          wxDefaultSize,
          wxT("Open Bitmap File Dialog")
     };

     if ( (fd.ShowModal() == wxID_OK)  ) {
          if ( fd.GetFilterIndex() == 0) { // png
               auto & app = wxGetApp();
               int32_t idx = app.get_view()->get_current_bitmap_lib_index();
               if (idx == -1) {
                    if (!app.get_document()->init_bitmap_lib(image_container::type::ImageLib)) {
                         wxMessageBox(wxT("Load bitmap_lib failed"));
                         return;
                    }  
               }
               wxString path = fd.GetPath();
               if (! app.get_document()->load_png_file(++idx,path)) {
                    wxMessageBox(wxString::Format(wxT("Load \"%s\" failed"),path.wc_str()));
               }else{
                  app.get_view()->set_current_bitmap_lib_index(idx);
                  osd_image::pos_type size;
                  app.get_document()->get_bitmap_size(idx,size);
                  app.get_view()->set_current_bitmap_size(size);
                  app.get_view()->Refresh();
               }
          } else {
               if ( fd.GetFilterIndex() == 1) {
                    wxMessageBox(wxT(".mcm file"));
               }
          }

     }

     // if (this->m_view->m_doc->is_modified()){

     //}

}



/*
 Timer represents the update rate of the  airborne telemetry unit
*/
void main_frame::OnTimer(wxTimerEvent &event)
{

}
