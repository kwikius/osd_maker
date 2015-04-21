#ifndef QUANTRACKER_SIM_PANEL_HPP_INCLUDED
#define QUANTRACKER_SIM_PANEL_HPP_INCLUDED

#include <wx/scrolwin.h>
#include <wx/treectrl.h>
#include <wx/arrstr.h>
//#include <map>

struct view;
struct project_tree;
struct panel : wxWindow// wxPanel
{
  panel (wxWindow * parent);
  void add_bitmap_handle(std::string const & name, int handle);
  void add_font_handle(std::string const & name, int handle);
  void set_project_name(wxString const & str);
  bool get_bitmap_handle(wxTreeEvent & event, int & result_out)const;
  bool get_font_handle(wxTreeEvent & event, int & result_out)const;
  void reset();
  wxArrayString get_bitmap_names()const;
  wxArrayString get_font_names()const;
  bool Destroy();
private:
   enum class selection_type{ Unknown, Font, FontDir, Bitmap, BitmapDir, LayoutDir};
   selection_type get_selection_type(wxTreeEvent& event);
   void on_font_item_activated(wxTreeEvent & event);
   void on_bitmap_item_activated(wxTreeEvent & event);
   void on_layout_dir_activated(wxTreeEvent & event);
   void on_bitmap_dir_activated( wxTreeEvent& event);
   void on_font_dir_activated( wxTreeEvent& event);
   void OnTreeItemActivated(wxTreeEvent & event);
   void OnTreeItemRightClick(wxTreeEvent & event);
   void rename_bitmap(wxTreeEvent & event);
   void on_bitmap_right_click(wxTreeEvent & event, int handle);
   void on_font_right_click(wxTreeEvent & event, int handle);
   void resize_font(int handle);
   enum
   {
      idMenuQuit = 1000,
      idMenuAbout,
      idTreeControl
   };
   project_tree* m_project_tree;

   DECLARE_EVENT_TABLE()

};

#endif // QUANTRACKER_SIM_PANEL_HPP_INCLUDED
