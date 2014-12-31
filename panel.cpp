
#include <wx/wx.h>

#include "app.h"
#include "window_ids.hpp"
#include "events.hpp"
#include "document.hpp"
#include "main_frame.h"
#include "view.hpp"
#include "panel.hpp"

panel::panel (wxWindow* parent)
:  wxScrolledWindow {parent},m_tree_ctrl{nullptr}

{
 int tree_style = wxTR_SINGLE | wxTR_HAS_BUTTONS | wxTR_EDIT_LABELS;
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


