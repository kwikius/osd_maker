

#include "view.hpp"
#include "panel.hpp"
#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

struct osd_handle : public wxTreeItemData {
   osd_handle(int handle) : m_osd_handle{handle}{}
   int get_handle()const{ return m_osd_handle;}
  private:
   int const m_osd_handle;
};

void panel::add_bitmap_handle(std::string const & name, int handle)
{
   m_tree_ctrl->AppendItem(m_bitmaps,quan::gx::wxwidgets::to_wxString(name)
   ,-1
   ,-1
   ,new osd_handle{handle}
   );
     
}

panel::panel (wxWindow* parent)
:  wxScrolledWindow {parent},m_tree_ctrl{nullptr}

{
 int tree_style = wxTR_SINGLE | wxTR_HAS_BUTTONS ;
 m_tree_ctrl = new wxTreeCtrl(this,idTreeControl, wxDefaultPosition, wxDefaultSize, tree_style,
 wxDefaultValidator, wxT("OSD Bitmap Project"));

 auto root = m_tree_ctrl->AddRoot(wxT("New Project"));
 m_fonts = m_tree_ctrl->AppendItem(root,wxT("fonts"));
 m_bitmaps = m_tree_ctrl->AppendItem(root,wxT("bitmaps"));

 m_tree_ctrl->Expand(root);
 m_tree_ctrl->SetSize(20,20,200,200);
 //m_tree_ctrl->Show();
}

BEGIN_EVENT_TABLE (panel, wxPanel)
  // EVT_BUTTON (idBtnConnect, panel::OnConnectDisconnect)
  // EVT_COMMAND_SCROLL (idScaleSlider,panel::OnScaleSlider)
END_EVENT_TABLE()


