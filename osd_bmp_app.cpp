/***************************************************************
 * Name:      OsdBmpApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2013-03-06
 * Copyright: Andy (http://www.softpainter.org)
 * License:
 **************************************************************/

//#ifdef WX_PRECOMP
//#include "wx_pch.h"
//#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__
#include <wx/cmdline.h>
#include <wx/filefn.h>
#include <wx/stdpaths.h>

#include <quan/uav/osd/dynamic/object_database.hpp>
#include "osd_bmp_app.hpp"
#include "document.hpp"
#include "gui/main_frame.hpp"
#include "gui/panel.hpp"
#include "gui/view.hpp"
#include "gui/main_frame_splitter.hpp"
#include "gui/font_preview.hpp"

IMPLEMENT_APP(OsdBmpApp);

void OsdBmpApp::init_mainframe()
{
    wxSize frame_size = wxDefaultSize;
    m_app_config->Read(wxT("/MainFrame/InitialWidth"),&frame_size.x);
    m_app_config->Read(wxT("/MainFrame/InitialHeight"),&frame_size.y);
    m_frame = new main_frame(0L, wxT("OSD BitmapMaker"),frame_size);
    m_frame->Show();
}

wxString OsdBmpApp::get_app_dir() const
{
  return wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
}

void OsdBmpApp::OnInitCmdLine(wxCmdLineParser & parser)
{
  //assert (false && __LINE__);
  parser.AddOption(wxT("p"),wxT("project"),wxT("open a project"));
}

bool OsdBmpApp::OnCmdLineParsed(wxCmdLineParser & parser)
{
    wxString path;
    if ( parser.Found(wxT("p"),&path)){
      return this->get_main_frame()->open_project (path);
    }
    return true;
}

bool OsdBmpApp::OnInit()
{
    init_mainframe();
    return wxApp::OnInit();
}

view* OsdBmpApp::get_view()const
{
   return m_frame->m_splitter->get_view();
}

font_preview* OsdBmpApp::get_font_preview()const
{
   return m_frame->m_splitter->get_tree_panel_splitter()->m_font_preview;
}

OsdBmpApp::OsdBmpApp()
: m_frame{nullptr}
, m_document{new document}
//, m_sp{nullptr}
, m_app_config {new wxConfig(wxT("OSD BitmapMaker"))}
{}

panel * OsdBmpApp::get_panel() const
{
   assert(
      (m_frame !=nullptr) &&
      (m_frame->m_splitter != nullptr) &&
      (m_frame->m_splitter->get_tree_panel_splitter() != nullptr) &&
      ( m_frame->m_splitter->get_tree_panel_splitter()->m_panel != nullptr)
   );
   return m_frame->m_splitter->get_tree_panel_splitter()->m_panel;
}


OsdBmpApp::~OsdBmpApp()
{
   delete m_document;
   delete m_app_config;
}
