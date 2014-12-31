
#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include "view.hpp"
#include "panel.hpp"
#include "bitmap_tree.hpp"

struct osd_handle : public wxTreeItemData {
   osd_handle(int handle) : m_osd_handle{handle}{}
   int get_handle()const{ return m_osd_handle;}
  private:
   int const m_osd_handle;
};

void panel::add_bitmap_handle(std::string const & name, int handle)
{
   m_bitmap_tree->AppendItem(
      m_bitmap_tree->get_bitmaps_id(),
      quan::gx::wxwidgets::to_wxString(name)
      ,-1
      ,-1
      ,new osd_handle{handle}
   );
}

panel::panel (wxWindow* parent)
:  wxScrolledWindow{parent},
  m_bitmap_tree{new bitmap_tree{this,idTreeControl}}
{
 m_bitmap_tree->Expand(m_bitmap_tree->GetRootItem());
 m_bitmap_tree->SetSize(20,20,200,200);
 //m_bitmap_tree->Show();
}

BEGIN_EVENT_TABLE (panel, wxPanel)
  // EVT_BUTTON (idBtnConnect, panel::OnConnectDisconnect)
  // EVT_COMMAND_SCROLL (idScaleSlider,panel::OnScaleSlider)
END_EVENT_TABLE()


