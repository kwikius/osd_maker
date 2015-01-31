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

#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>
#include "../document.hpp"
//#include "sp_in_thread.hpp"
#include "../aircraft/aircraft.hpp"

#include "../osd_bmp_app.hpp"

#include "main_frame.hpp"
#include "main_frame_splitter.hpp"
//#include "../comm/sp_in_thread.hpp"

#include "dialogs/new_bitmap_dialog.hpp"
#include "dialogs/bitmap_resize_dialog.hpp"

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

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
   EVT_MENU (wxID_SAVEAS,main_frame::OnSaveProjectAs)
   EVT_MENU (idImportBitmap, main_frame::OnImportBitmap)
   EVT_MENU (idNewBitmap, main_frame::OnNewBitmap)
   EVT_MENU (idImportFont, main_frame::OnImportFont)
   EVT_MENU (idCommitViewToTree, main_frame::OnCommitViewToTree)
   EVT_MENU (idResizeViewBitmap,main_frame::OnResizeViewBitmap)
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

     Timer= new wxTimer{this,idTimer};
//    update rate of 1/50th sec
     Timer->Start(20,wxTIMER_CONTINUOUS);
   
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
   enable_menu_item (idImportBitmap, b);
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
   
   wxMenu* bitmapMenu = new wxMenu (_T (""));
   mbar->Append (bitmapMenu, _ ("&Bitmap"));
   bitmapMenu->Append(idNewBitmap, _ ("&New..."), _ ("New Bitmap"));
   bitmapMenu->Append(idImportBitmap, _ ("&Import..."), _ ("Import Bitmap"));
   bitmapMenu->Append(idResizeViewBitmap,  _ ("Resize current bitmap..."));

   wxMenu* fontMenu = new wxMenu (_T (""));
   mbar->Append (fontMenu, _ ("&Font")); 
   fontMenu->Append (idImportFont, _ ("&Import..."), _ ("Import Font"));
   
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
#if 0
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
#endif
   return wxFrame::Destroy();
}

void main_frame::OnResizeViewBitmap(wxCommandEvent & event)
{
//  {5,-5,-5,0};
//  
   auto view = wxGetApp().get_view();
   if (view->have_image() && (view->get_view_mode() == ::view::view_mode::inBitmaps)){
   bitmap_resize_dialog dlg(this);
   if ( dlg.ShowModal() == wxID_OK){
      quan::two_d::box<long> new_size;
      dlg.m_left_incr->GetValue().ToLong(&new_size.left);
      dlg.m_top_incr->GetValue().ToLong(&new_size.top);
      dlg.m_right_incr->GetValue().ToLong(&new_size.right);
      dlg.m_bottom_incr->GetValue().ToLong(&new_size.bottom);

      quan::two_d::box<int> new_size1;
      new_size1.top  =static_cast<int>(new_size.top *-1);
      new_size1.bottom =static_cast<int>(new_size.bottom *-1);
      new_size1.left  =static_cast<int>(new_size.left);
      new_size1.right =static_cast<int>(new_size.right);
      wxGetApp().get_view()->resize_image(new_size1);
   }
   }
   
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
               app.get_font_preview()->Refresh();
            }
         else {
            wxMessageBox(wxT("Unknown File type"));
         }
      }
}

void main_frame::OnNewBitmap (wxCommandEvent &event)
{
   new_bitmap_dialog dlg(this);
   quan::two_d::vect<long> size;
   
   if ( dlg.ShowModal() == wxID_OK){
      if (! dlg.m_x_text->GetValue().ToLong(&size.x)){
         wxMessageBox(wxT("invalid number in x"));
         return;
      }
      if (! dlg.m_y_text->GetValue().ToLong(&size.y)){
         wxMessageBox(wxT("invalid number in y"));
         return;
      }
      wxString wxname = dlg.m_name_text->GetValue(); //
      std::string name = from_wxString<char>(wxname);
      if ( wxGetApp().get_document()->get_bitmap(name) != nullptr){
         wxMessageBox(wxT("Bitmap name already exists"));
         return;
      }
      wxGetApp().get_document()->add_new_bitmap(name,size);
   }
}

void main_frame::OnImportBitmap (wxCommandEvent &event)
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
               app.get_font_preview()->Refresh();
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

void main_frame::OnSaveProjectAs (wxCommandEvent &event)
{
   wxFileDialog fd(
      this,
      wxT("Save OSD Project File"), 
      wxT(""), 
      wxT(""),
      wxT("ZIP files (*.zip)|*.zip"),
      wxFD_SAVE | wxFD_OVERWRITE_PROMPT
   );

   if ( fd.ShowModal() == wxID_OK){
     wxGetApp().get_document()->save_project_as(fd.GetPath());
   }
}

void main_frame::clear()
{
   wxGetApp().get_panel()->reset();
   wxGetApp().get_font_preview()->reset();
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

void main_frame::OnTimer (wxTimerEvent &event)
{
   auto view = wxGetApp().get_view();
   if ( view->get_view_mode() == ::view::view_mode::inLayouts){
      auto bearing = the_aircraft.get_heading();
      bearing += quan::angle::deg{ 2};
      if ( bearing > quan::angle::deg{360}){
         bearing -= quan::angle::deg{360};
      }
      the_aircraft.set_heading(bearing);

//      auto home_bearing = wxGetApp().get_view()->get_home_bearing();
//      home_bearing += quan::angle::deg{ 2.2};
//      if ( home_bearing > quan::angle::deg{360}){
//         home_bearing -= quan::angle::deg{360};
//      }
//      view->set_home_bearing(home_bearing);
//
       
       view->Refresh();
    
   }
}

void main_frame::OnAbout (wxCommandEvent &event)
{
   wxString msg = wxbuildinfo (long_f);
   wxMessageBox (msg, _ ("Welcome to..."));
}
