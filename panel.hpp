#ifndef QUANTRACKER_SIM_PANEL_HPP_INCLUDED
#define QUANTRACKER_SIM_PANEL_HPP_INCLUDED


//#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/treectrl.h>
#include "osd_image.hpp"

struct view;
struct panel : wxScrolledWindow// wxPanel
{
  panel (wxWindow * parent); 
private:

   enum
   {
      idMenuQuit = 1000,
      idMenuAbout,
      idTreeControl
   };
   wxTreeCtrl* m_tree_ctrl;
   wxTreeItemId m_fonts;
   wxTreeItemId m_bitmaps;
   DECLARE_EVENT_TABLE()
};

#endif // QUANTRACKER_SIM_PANEL_HPP_INCLUDED
