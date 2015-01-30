#ifndef OSD_BITMAP_MAKER_TREE_HPP_INCLUDED
#define OSD_BITMAP_MAKER_TREE_HPP_INCLUDED

#include <wx/treectrl.h>

struct project_tree : wxTreeCtrl{
   project_tree( wxWindow* parent, int id);
   wxTreeItemId const & get_bitmaps_id()const {return m_bitmaps;}
   wxTreeItemId const & get_fonts_id()const {return m_fonts;}
   wxTreeItemId const & get_layouts_id() const { return m_layouts;}
private:
   wxTreeItemId m_fonts;
   wxTreeItemId m_bitmaps;
   wxTreeItemId m_layouts;
  // DECLARE_EVENT_TABLE()
};

#endif // OSD_BITMAP_MAKER_TREE_HPP_INCLUDED
