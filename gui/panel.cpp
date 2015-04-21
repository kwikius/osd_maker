#define wxUSE_CHOICEDLG 1
#include <wx/choicdlg.h>
#include <wx/textdlg.h>

#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include "view.hpp"
#include "panel.hpp"
#include "project_tree.hpp"
#include "font_preview.hpp"
#include "dialogs/bitmap_resize_dialog.hpp"

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

wxArrayString panel::get_font_names()const
{
   auto fontid = m_project_tree->get_fonts_id();
   wxTreeItemIdValue cookie;
   wxArrayString result;
   auto child = m_project_tree->GetFirstChild(fontid,cookie);
   while ( child.IsOk()){
      wxString str = m_project_tree->GetItemText(child);
      result.Add(str);
      child = m_project_tree->GetNextChild(fontid,cookie);
   }
   return result;
}

wxArrayString panel::get_bitmap_names()const
{
   auto bmpid = m_project_tree->get_bitmaps_id();
   wxTreeItemIdValue cookie;

   wxArrayString result;
   auto child = m_project_tree->GetFirstChild(bmpid,cookie);
   while ( child.IsOk()){
      wxString str = m_project_tree->GetItemText(child);
      result.Add(str);
      child = m_project_tree->GetNextChild(bmpid,cookie);
   }
   return result;
}

void panel::reset()
{
   delete m_project_tree ;
   m_project_tree = new project_tree{this,idTreeControl};
   m_project_tree->Expand(m_project_tree->GetRootItem());
   m_project_tree->SetSize(20,20,200,200);
}

void panel::set_project_name(wxString const & str)
{
  auto root = m_project_tree->GetRootItem();
  m_project_tree->SetItemText(root,str);
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
   this->Refresh();
}

bool panel::Destroy()
{
    m_project_tree->Destroy();
    return wxWindow::Destroy();
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

  view::dynamic_bitmap* abc_bmp = wxGetApp().get_document()->get_bitmap(handle);
  view::dynamic_bitmap* bmp = dynamic_cast<view::dynamic_bitmap*>(abc_bmp);
  if ( bmp == nullptr){
      return ;
  }
  wxString old_name = to_wxString(bmp->get_name());
  // do a text dialog to get name
  wxTextEntryDialog dlg(this,wxT("Rename Bitmap"),
      wxT("Enter new name for bitmap"),old_name);

  if ( dlg.ShowModal() == wxID_OK){
    wxString wx_new_text = dlg.GetValue();
    wxTreeItemId id = event.GetItem();
    m_project_tree->SetItemText(id, wx_new_text);
    bmp->set_name(from_wxString<char>(wx_new_text));
    wxGetApp().get_document()->set_modified(true);
  }
}

void panel::on_bitmap_right_click(wxTreeEvent & event, int handle)
{
   wxString choices[] =
   {
      wxT("Rename")
     ,wxT("Delete")
   };
   int style = wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE;
   wxSingleChoiceDialog dlg{
      this
      ,wxT("Bitmap Tree Item Actions")
      ,wxT("Bitmap- Tree Item Actions")
      ,sizeof(choices) / sizeof(wxString)
      ,(wxString*)choices
#if wxCHECK_VERSION(3,0,0)
      ,(void**)NULL
#else
      , NULL
#endif
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

void panel::resize_font(int font_handle)
{

   bitmap_resize_dialog dlg{this, wxT("Resize Font Dialog")};

    if ( dlg.ShowModal() == wxID_OK){
      quan::two_d::box<long> new_size;
      dlg.m_left_incr->GetValue().ToLong(&new_size.left);
      dlg.m_top_incr->GetValue().ToLong(&new_size.top);
      dlg.m_right_incr->GetValue().ToLong(&new_size.right);
      dlg.m_bottom_incr->GetValue().ToLong(&new_size.bottom);

      quan::two_d::box<int> new_size1;
      new_size1.top  =static_cast<int>(new_size.top *-1);
      new_size1.bottom =static_cast<int>(new_size.bottom *-1);
      new_size1.left  =static_cast<int>(new_size.left);
      new_size1.right =static_cast<int>(new_size.right);

     // wxMessageBox(wxT("dummy resize"));
     auto pdoc = wxGetApp().get_document();
     db_font* font = pdoc->get_font(font_handle);
     assert(font && __LINE__);
     // for each bitmap in the font
     // resize the bitmap
     auto view = wxGetApp().get_view();
     bool view_image_not_done = view->have_image();
     // TODO
      // 1 Check that size is >= 2 in any direction
      // 
     for ( size_t i = font->get_begin();
            i < (font->get_num_elements() + static_cast<size_t>(font->get_begin()));
            ++i) {
         int image_handle = -1;
         bool result = font->get_handle_at( i, image_handle);
         assert( result && __LINE__);
         assert(image_handle != -1);
         quan::uav::osd::dynamic::bitmap* font_elem = pdoc->get_bitmap(image_handle);
         assert(font_elem && __LINE__);
         font_elem->resize(new_size1);
         
         if ( (view_image_not_done == true)  && (view->get_doc_image_handle() == image_handle)){
            view->resize_image(new_size1);
            view_image_not_done = false;
           // view->Refresh();
         }
     }
      auto font_preview = wxGetApp().get_font_preview();
      if ( font_preview->get_font() == font){
        font_preview->Refresh();
      }
      
   }
}

void panel::on_font_right_click(wxTreeEvent & event, int handle)
{
    wxString choices[] =
   {
      wxT("Rename")
     ,wxT("Delete")
     ,wxT("Resize")
   };
   int style = wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE;
   wxSingleChoiceDialog dlg{
      this
      ,wxT("Font Tree Item Actions")
      ,wxT("Font Tree Item Actions")
      ,sizeof(choices) / sizeof(wxString)
      ,(wxString*)choices
#if wxCHECK_VERSION(3,0,0)
      ,(void**)NULL
#else
      , NULL
#endif
      ,style
   };

   if (dlg.ShowModal() == wxID_OK){
      int result = dlg.GetSelection();
      switch (result){
         case 0:
            // rename
         break;
         case 1:
        //  delete font
         break;
         case 2:
          resize_font(handle);
         break;
         default:
         break;
      }
   } 
}
void panel::OnTreeItemRightClick(wxTreeEvent & event)
{
  int handle = -1;
  if ( get_bitmap_handle( event,handle)){
     on_bitmap_right_click(event,handle);
  }else{
    if (get_font_handle(event,handle)){
      on_font_right_click(event,handle);
    }else{
      return;
    }
  }
}




