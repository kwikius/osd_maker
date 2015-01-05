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
#include "main_frame_splitter.hpp"
#include "sp_in_thread.hpp"

//helper functions
enum wxbuildinfoformat {
   short_f, long_f
};

wxString wxbuildinfo (wxbuildinfoformat format)
{
   wxString wxbuild (wxVERSION_STRING);
   
   if (format == long_f) {
#if defined(__WXMSW__)
         wxbuild << _T ("-Windows");
#elif defined(__WXMAC__)
         wxbuild << _T ("-Mac");
#elif defined(__UNIX__)
         wxbuild << _T ("-Linux");
#endif
         
#if wxUSE_UNICODE
         wxbuild << _T ("-Unicode build");
#else
         wxbuild << _T ("-ANSI build");
#endif // wxUSE_UNICODE
      }
      
   return wxbuild;
}

BEGIN_EVENT_TABLE (main_frame, wxFrame)
   EVT_CLOSE (main_frame::OnCloseProject)
   EVT_MENU (idMenuQuit, main_frame::OnQuit)
   EVT_MENU (idMenuAbout, main_frame::OnAbout)
   EVT_MENU (wxID_NEW, main_frame::OnNewProject)
   EVT_MENU (wxID_OPEN, main_frame::OnOpenProject)
   EVT_MENU (wxID_SAVE, main_frame::OnSaveProject)
   EVT_MENU (idImportImage, main_frame::OnImportImage)
   EVT_MENU (idImportFont, main_frame::OnImportFont)
   EVT_MENU (idCommitViewToTree, main_frame::OnCommitViewToTree)
   EVT_TIMER (idTimer, main_frame::OnTimer)
END_EVENT_TABLE()

main_frame::main_frame (wxFrame *frame, const wxString& title, wxSize const & size)
   : wxFrame (frame, -1, title, wxDefaultPosition, size)
   , m_splitter {nullptr}
, Timer {nullptr}
, m_sp_in_thread {nullptr} {

   this->m_splitter = new main_frame_splitter (this);
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

void main_frame::enable_menu_item (int id, bool b)
{
   auto mbar = GetMenuBar();
   assert (mbar);
   mbar->Enable (id, b);
}

void main_frame::enable_import_image (bool b)
{
   enable_menu_item (idImportImage, b);
}
void main_frame::enable_import_font (bool b)
{
   enable_menu_item (idImportFont, b);
}
void main_frame::enable_save_project (bool b)
{
   enable_menu_item (wxID_SAVE, b);
}
void main_frame::enable_save_project_as (bool b)
{
   enable_menu_item (wxID_SAVEAS, b);
}

void main_frame::enable_commit_view_to_tree (bool b)
{
   enable_menu_item (idCommitViewToTree, b);
}

void main_frame::create_menus()
{
   wxMenuBar* mbar = new wxMenuBar();
   SetMenuBar (mbar);
   
   wxMenu* projectMenu = new wxMenu (_T (""));
   mbar->Append (projectMenu, _ ("&File"));
   projectMenu->Append (wxID_NEW, _ ("&New Project...\tCtrl+N"), _ ("New Project"));
   projectMenu->Append (wxID_OPEN, _ ("&Open Project...\tCtrl+O"), _ ("Open Project"));
   projectMenu->Append (wxID_SAVE, _ ("&Save Project\tCtrl+S"), _ ("Save Project"));
   projectMenu->Append (wxID_SAVEAS, _ ("&Save Project As...\tShift+Ctrl+S"), _ ("Save Project As"));
   projectMenu->Append (idMenuQuit, _ ("&Quit\tCtrl+Q"), _ ("Quit the Application"));
   
   wxMenu* fileMenu = new wxMenu (_T (""));
   mbar->Append (fileMenu, _ ("&Import"));
   fileMenu->Append (idImportImage, _ ("&Image..."), _ ("Import Image File"));
   fileMenu->Append (idImportFont, _ ("&Font..."), _ ("Import Font File"));
   
   wxMenu* viewMenu = new wxMenu (_T (""));
   mbar->Append (viewMenu, _ ("&View"));
   viewMenu->Append (idCommitViewToTree, _ ("Commit view to live-tree"));
   
   wxMenu* helpMenu = new wxMenu (_T (""));
   mbar->Append (helpMenu, _ ("&Help"));
   helpMenu->Append (idMenuAbout, _ ("&About"), _ ("Show info about this application"));
   
   enable_save_project (false);
   enable_save_project_as (false);
   enable_commit_view_to_tree (false);
   //   enable_import_image(false);
   //  enable_import_font(false);
   enable_import_image (true);
}


void main_frame::create_statusbar()
{
   CreateStatusBar (2);
   SetStatusText (wxT ("Welcome to OSD BitmapMaker"), 0);
   SetStatusText (wxT ("V1.0"), 1);
}

bool main_frame::Destroy()
{
   {
      wxCriticalSectionLocker lock (m_thread_CS);
      if (m_sp_in_thread != nullptr) {
            m_sp_in_thread->Delete();
         };
   }
   
   for (;;) {
         wxCriticalSectionLocker lock (m_thread_CS);
         if (m_sp_in_thread == nullptr) {
               break;
            }
      }
   return wxFrame::Destroy();
}

void main_frame::OnCommitViewToTree (wxCommandEvent & event)
{
   wxGetApp().get_view()->sync_to_document();
}

void main_frame::OnCloseProject (wxCloseEvent &event)
{
   auto & app = wxGetApp();
   if (event.CanVeto() && (app.get_document()->is_modified() || app.get_view()->is_modified())) {
         if (wxMessageBox (wxT ("Do you want to save before exit?"),
                           wxT ("Confirm Library File Save"),
                           wxICON_QUESTION | wxYES_NO) == wxYES) {
               wxGetApp().get_document()->save_project();
            }
      }
   wxSize frame_size = this->GetSize();
   auto config = wxGetApp().get_config();
   config->Write (wxT ("/MainFrame/InitialWidth"), frame_size.x);
   config->Write (wxT ("/MainFrame/InitialHeight"), frame_size.y);
   
   Destroy();
}

void main_frame::OnQuit (wxCommandEvent &event)
{
   event.Skip();
   this->Close();
}

//TODO |MinimOSD Charset(*.mcm)|*.mcm
void main_frame::OnImportFont (wxCommandEvent &event)
{
   auto & app = wxGetApp();
   auto doc = app.get_document();
   wxFileDialog fd {
      this,
      wxT ("Import Image"), // message
      wxT (""),                    // default dir
      wxT (""),                    // default file
      wxT ("MCM files(*.mcm)|*.mcm"), // wildcard
      wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR
   };
   if ( (fd.ShowModal() == wxID_OK)) {
         if (fd.GetFilterIndex() == 0) {  // png
               wxString path = fd.GetPath();
               if (! doc->load_mcm_font_file (path)) {
                     wxMessageBox (wxString::Format (wxT ("Load \"%s\" failed"), path.wc_str()));
                     return;
                  }
               app.get_bitmap_preview()->Refresh();
            }
         else {
            wxMessageBox(wxT("Unknown File type"));
         }
      }
}

void main_frame::OnImportImage (wxCommandEvent &event)
{
   auto & app = wxGetApp();
   auto doc = app.get_document();
   wxFileDialog fd {
      this,
      wxT ("Import Image"), // message
      wxT (""),                    // default dir
      wxT (""),                    // default file
      wxT ("PNG files(*.png)|*.png"), // wildcard
      wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR
   };
   if ( (fd.ShowModal() == wxID_OK)) {
         if (fd.GetFilterIndex() == 0) {  // png
               wxString path = fd.GetPath();
               if (! doc->load_png_file (path)) {
                     wxMessageBox (wxString::Format (wxT ("Load \"%s\" failed"), path.wc_str()));
                     return;
                  }
               app.get_bitmap_preview()->Refresh();
            }
         else {
            wxMessageBox(wxT("Unknown File type"));
         }
      }
}

void main_frame::OnSaveProject (wxCommandEvent &event)
{
   wxGetApp().get_document()->save_project();
}

void main_frame::clear()
{
   wxGetApp().get_panel()->reset();
   wxGetApp().get_bitmap_preview()->reset();
   wxGetApp().get_view()->reset();
   wxGetApp().get_document()->reset();
}

void main_frame::OnNewProject (wxCommandEvent &event)
{
   //TODO if current project modified then save

   this->clear();
   this->Refresh();
#if 0
   // dialog with project name
   wxTextEntryDialog dlg {
      this,
      wxT ("Enter Project name"),
      wxT ("New Project"),
      wxT ("OSDBitmaps1")
   };
   
   if (dlg.ShowModal() == wxID_OK) {
   
         // if have current project
         wxGetApp().get_document()->set_project_name (dlg.GetValue());
      }
#endif
}

void main_frame::OnOpenProject (wxCommandEvent &event)
{
   wxFileDialog dlg (this, wxT ("Open Project"), wxT (""), wxT ("")
                     , wxT ("zip files(*zip)|*.zip"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
   if (dlg.ShowModal() == wxID_CANCEL) {
         return;
      }
   wxString path = dlg.GetPath();
   wxGetApp().get_document()->open_project (path);
}

/*
 Timer represents the update rate of the  airborne telemetry unit
*/
void main_frame::OnTimer (wxTimerEvent &event)
{

}

void main_frame::OnAbout (wxCommandEvent &event)
{
   wxString msg = wxbuildinfo (long_f);
   wxMessageBox (msg, _ ("Welcome to..."));
}

