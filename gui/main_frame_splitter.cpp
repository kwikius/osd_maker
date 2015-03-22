#include "main_frame_splitter.hpp"

main_frame_splitter::main_frame_splitter(wxWindow* parent) : wxSplitterWindow(parent, wxID_ANY)
{
   SetSashGravity(0.5);
   SetSashPosition(2000);
   SetMinimumPaneSize(400);
   m_view = new view(this);
   m_tree_panel_splitter = new tree_panel_splitter(this);
   SplitVertically(m_tree_panel_splitter,m_view);
}

bool main_frame_splitter::Destroy()
{
   m_view->Destroy();
   m_tree_panel_splitter->Destroy();
   return wxSplitterWindow::Destroy();
}
