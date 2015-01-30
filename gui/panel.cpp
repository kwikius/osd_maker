#define wxUSE_CHOICEDLG 1
#include <wx/choicdlg.h>
#include <wx/textdlg.h>

#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include "view.hpp"
#include "panel.hpp"
#include "project_tree.hpp"
#include "font_preview.hpp"

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

namespace {
struct osd_bitmap_handle : public wxTreeItemData {
   osd_bitmap_handle(int handle) : m_handle{handle}{}
   int get_handle()const{ return m_handle;}
  private:
   int const m_handle;
};

struct osd_font_handle : public wxTreeItemData {
   osd_font_handle(int handle) : m_handle{handle}{}
   int get_handle()const{ return m_handle;}
  private:
   int const m_handle;
};

} // namespace 


void panel::reset()
{
   delete m_project_tree ;
   m_project_tree = new project_tree{this,idTreeControl};
   m_project_tree->Expand(m_project_tree->GetRootItem());
   m_project_tree->SetSize(20,20,200,200); 
}

// todo add modes for font and bitmaps parents selected
// need to change view mode if necessary
panel::selection_type panel::get_selection_type( wxTreeEvent & event)
{
    auto id = event.GetItem();
    auto parent_id = m_project_tree->GetItemParent(id);
    if( parent_id == m_project_tree->get_bitmaps_id()){
         return selection_type::Bitmap;
    }
    if(parent_id == m_project_tree->get_fonts_id()){
         return selection_type::Font;
    }

    if(id == m_project_tree->get_layouts_id()){
            return selection_type::LayoutDir;
    }
    if ( id == m_project_tree->get_fonts_id()){
           return selection_type::FontDir;
    }
    if ( id == m_project_tree->get_bitmaps_id()){
           return selection_type::BitmapDir;
    }
    return selection_type::Unknown;
}

void panel::add_bitmap_handle(std::string const & name, int handle)
{
   m_project_tree->AppendItem(
      m_project_tree->get_bitmaps_id(),
      quan::gx::wxwidgets::to_wxString(name)
      ,-1
      ,-1
      ,new osd_bitmap_handle{handle}
   );
}
void panel::add_font_handle(std::string const & name, int handle)
{
   m_project_tree->AppendItem(
      m_project_tree->get_fonts_id(),
      quan::gx::wxwidgets::to_wxString(name)
      ,-1
      ,-1
      ,new osd_font_handle{handle}
   );
   auto font_preview  = wxGetApp().get_font_preview();
   int preview_handle = font_preview->get_font_handle();
   if ( preview_handle == -1){
      wxGetApp().get_font_preview()->set_font_handle(handle);
   }
}

panel::panel (wxWindow* parent)
//:  wxScrolledWindow{parent},
 : wxWindow{parent,wxID_ANY},
  m_project_tree{new project_tree{this,idTreeControl}}
{
 m_project_tree->Expand(m_project_tree->GetRootItem());
 m_project_tree->SetSize(20,20,200,200);

}

BEGIN_EVENT_TABLE (panel, wxWindow)
  EVT_TREE_ITEM_ACTIVATED(idTreeControl,panel::OnTreeItemActivated)
  EVT_TREE_ITEM_RIGHT_CLICK(idTreeControl,panel::OnTreeItemRightClick)
END_EVENT_TABLE()

bool panel::get_bitmap_handle(wxTreeEvent & event, int & result_out)const
{
   auto id = event.GetItem();
   auto abc_data = m_project_tree->GetItemData(id);
// assert?
   if ( abc_data == nullptr){
      return false;
   }
   auto d = dynamic_cast<osd_bitmap_handle*>(abc_data);
      if ( d == nullptr){
      return false;
   }
   result_out = d->get_handle();
   return true;
}

bool panel::get_font_handle(wxTreeEvent & event, int & result_out)const
{
   auto id = event.GetItem();
   auto abc_data = m_project_tree->GetItemData(id);
   if ( abc_data == nullptr){
      return false;
   }
   auto fh = dynamic_cast<osd_font_handle*>(abc_data);
      if ( fh == nullptr){
      return false;
   }
   result_out = fh->get_handle();
   return true;
}

void panel::OnTreeItemActivated(wxTreeEvent & event)
{
  // find out if font or bitmap
   auto seltype = get_selection_type(event);
   switch(seltype){
      case selection_type::Bitmap:
         on_bitmap_item_activated(event);
      break;
      case selection_type::Font:
         on_font_item_activated(event);
      break;
      case selection_type::LayoutDir: 
         on_layout_dir_activated(event); 
      break;
      case selection_type::FontDir:
         on_font_dir_activated(event);
      break;
      case selection_type::BitmapDir:
         on_bitmap_dir_activated(event);
      break;
      default:
      break;
   }

}

void panel::on_font_item_activated(wxTreeEvent & event)
{
   // then we need to select the selected font into the preview
   // whatever is in the view can stay until a new item is selected into the view
   // so its previews job to sort it
   // put its handle to font_preview
   int font_handle  = -1;
   if ( ! get_font_handle(event, font_handle)){
      return ;
   }
   wxGetApp().get_font_preview()->set_font_handle(font_handle);
   auto view = wxGetApp().get_view();
   if ( view->get_view_mode() == view::view_mode::inLayouts){
       view->Refresh();
   }
}

void panel::on_bitmap_item_activated(wxTreeEvent & event)
{
   int event_handle=-1;
   if ( ! get_bitmap_handle(event,event_handle)){
      return;
   }
   wxGetApp().get_view()->sync_with_image_handle(event_handle);
}

void panel::on_bitmap_dir_activated(wxTreeEvent & event)
{
   auto view = wxGetApp().get_view();
   if ( view->get_view_mode() == view::view_mode::inLayouts){
       view->set_view_mode(view::view_mode::inBitmaps);
       view->Refresh();
   }
}

void panel::on_font_dir_activated(wxTreeEvent & event)
{
   auto view = wxGetApp().get_view();
   if ( view->get_view_mode() == view::view_mode::inLayouts){
       view->set_view_mode(view::view_mode::inBitmaps);
       view->Refresh();
   }
}

void panel::on_layout_dir_activated(wxTreeEvent & event)
{
    auto view = wxGetApp().get_view();
    if ( view->get_view_mode() != view::view_mode::inLayouts){
       view->set_view_mode(view::view_mode::inLayouts);
       view->Refresh();
    }
   //wxMessageBox(wxT("layouts"));
}

void panel::rename_bitmap(wxTreeEvent & event)
{
  int handle = -1;
  if ( !get_bitmap_handle( event,handle)){
      return;
  }

  osd_image* abc_bmp = wxGetApp().get_document()->get_image(handle);
  osd_bitmap* bmp = dynamic_cast<osd_bitmap*>(abc_bmp);
  if ( bmp == nullptr){
      return ;
  }
  wxString old_name = to_wxString(bmp->get_name());
  // do a text dialog to get name
  wxTextEntryDialog dlg(this,wxT("Rename Bitmap"),wxT("Enter new name for bitmap"),old_name);

  if ( dlg.ShowModal() == wxID_OK){
    wxString wx_new_text = dlg.GetValue();
    wxTreeItemId id = event.GetItem();
    m_project_tree->SetItemText(id, wx_new_text);
    bmp->set_name(from_wxString<char>(wx_new_text));
    wxGetApp().get_document()->set_modified(true);
  }
}

void panel::OnTreeItemRightClick(wxTreeEvent & event)
{
  int handle = -1;
  if ( !get_bitmap_handle( event,handle)){
      return;
  }
  wxString choices[] =
  {
      wxT("Rename")
     ,wxT("Delete")
  };
  int style = wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE;
  wxSingleChoiceDialog dlg{ 
      this
      ,wxT("Tree Item Actions")
      ,wxT("Tree Item Actions")
      ,sizeof(choices) / sizeof(wxString)
      ,choices
      ,NULL
      ,style
  };

  if (dlg.ShowModal() == wxID_OK){
      int result = dlg.GetSelection();

      switch (result){
         case 0:
            rename_bitmap(event);

         break;
         case 1:
        //  delete
         break;

      }
      
  }
}




