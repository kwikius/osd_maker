#ifndef QUANTRACKER_SIM_PANEL_HPP_INCLUDED
#define QUANTRACKER_SIM_PANEL_HPP_INCLUDED

#include <wx/scrolwin.h>
#include <wx/treectrl.h>
#include <map>

struct view;
struct bitmap_tree;
struct panel : wxWindow// wxPanel
{
  panel (wxWindow * parent);
  void add_bitmap_handle(std::string const & name, int handle); 
  bool get_handle(wxTreeEvent & event, int & result_out)const;
  void reset();
private:
  
   void OnTreeItemActivated(wxTreeEvent & event);
   void OnTreeItemRightClick(wxTreeEvent & event);
   enum
   {
      idMenuQuit = 1000,
      idMenuAbout,
      idTreeControl
   };
   bitmap_tree* m_bitmap_tree;
  
   DECLARE_EVENT_TABLE()
   
};

#endif // QUANTRACKER_SIM_PANEL_HPP_INCLUDED
