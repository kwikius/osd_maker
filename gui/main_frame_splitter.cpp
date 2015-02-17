#include "main_frame_splitter.hpp"

main_frame_splitter::main_frame_splitter(wxWindow* parent) : wxSplitterWindow(parent, wxID_ANY)
{
   SetSashGravity(0.5);
   SetSashPosition(2000);
   SetMinimumPaneSize(400);
   m_view = new view(this);
   m_panel_splitter = new panel_splitter(this);
   SplitVertically(m_view,m_panel_splitter);
}

bool main_frame_splitter::Destroy()
{
   m_view->Destroy();
   m_panel_splitter->Destroy();
   return wxSplitterWindow::Destroy();
}
