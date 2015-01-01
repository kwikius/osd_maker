#include "bitmap_tree.hpp"

bitmap_tree::bitmap_tree( wxWindow* parent, int id)
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
}

//BEGIN_EVENT_TABLE(bitmap_tree,wxTreeCtrl)
//  
//END_EVENT_TABLE()