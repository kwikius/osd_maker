#include "project_tree.hpp"

project_tree::project_tree( wxWindow* parent, int id)
   :wxTreeCtrl{
      parent,
      id,
      wxDefaultPosition,
      wxDefaultSize,
      wxTR_SINGLE | wxTR_HAS_BUTTONS | wxTR_EDIT_LABELS,
      wxDefaultValidator,
     wxT("OSD Bitmap Tree")}
{
  auto root = this->AddRoot(wxT("New Project"));
  m_fonts = this->AppendItem(root,wxT("fonts"));
  m_bitmaps = this->AppendItem(root,wxT("bitmaps"));
  m_layouts = this->AppendItem(root,wxT("layouts"));
}

bool project_tree::Destroy()
{
    return wxTreeCtrl::Destroy();
}

//BEGIN_EVENT_TABLE(project_tree,wxTreeCtrl)
//
//END_EVENT_TABLE()
