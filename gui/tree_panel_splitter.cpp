#include "tree_panel_splitter.hpp"

tree_panel_splitter::tree_panel_splitter(wxWindow* parent) : wxSplitterWindow(parent, wxID_ANY)
{
   SetSashGravity(0.75);
   SetMinimumPaneSize(200);
   m_panel = new panel(this);
   m_font_preview = new font_preview(this);
   SplitHorizontally(m_panel, m_font_preview);
}

bool tree_panel_splitter::Destroy()
{
   m_panel->Destroy();
   m_font_preview->Destroy();
   return wxSplitterWindow::Destroy();
}
