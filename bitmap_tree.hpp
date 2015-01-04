#ifndef OSD_BITMAP_MAKER_TREE_HPP_INCLUDED
#define OSD_BITMAP_MAKER_TREE_HPP_INCLUDED

#include <wx/treectrl.h>

struct bitmap_tree : wxTreeCtrl{
   bitmap_tree( wxWindow* parent, int id);
   wxTreeItemId const & get_bitmaps_id(){return m_bitmaps;}
   wxTreeItemId const & get_fonts_id(){return m_fonts;}
private:
   wxTreeItemId m_fonts;
   wxTreeItemId m_bitmaps;
  // DECLARE_EVENT_TABLE()
};

#endif // OSD_BITMAP_MAKER_TREE_HPP_INCLUDED
