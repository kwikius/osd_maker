
#include "osd_object_database.hpp"
#include <cstdio>

quan::uav::osd::dynamic::bitmap * osd_object_database::get_bitmap_at(size_t i) const
{
   int handle = -1;
   if ( get_bitmap_handle_at(i,handle) == true){
      return find_bitmap_by_handle(handle);
   }else{
      return nullptr;
   }
}

quan::uav::osd::dynamic::font * osd_object_database::get_font_at(size_t i) const
{
   int handle = -1;
   if ( get_font_handle_at(i,handle) == true){
      return find_font_by_handle(handle);
   }else{
      return nullptr;
   }
}

bool osd_object_database::get_bitmap_handle_at(size_t i, int & handle_out)const
{
   if ( i >= get_num_bitmaps()){
      return false;
   }
   int handle = m_bitmaps.at(i);
   assert((handle != -1) && __LINE__);
   handle_out = handle;
   return true;
}

bool osd_object_database::get_font_handle_at(size_t i, int & handle_out) const
{
   if ( i >= get_num_fonts()){
      return false;
   }
   int handle = m_fonts.at(i);
   assert((handle != -1) && __LINE__);
   handle_out = handle;
   return true;
}

osd_object_database::dynamic_bitmap*
osd_object_database::find_bitmap_by_name(std::string const & name_in)const
{

    for( auto handle : m_bitmaps){
         assert(( handle != -1) && __LINE__);
         auto image = find_bitmap_by_handle(handle);
         assert(( image != nullptr) && __LINE__);
         auto bmp = dynamic_cast<dynamic_bitmap*>( image);
         if(bmp && (name_in == bmp->get_name())){
             return bmp;
         }
    }
    return nullptr;
}

db_font*
osd_object_database::find_font_by_name(std::string const & name_in)const
{
    for( auto v : m_font_map){
         db_font* f = v.second;
         if( (name_in == f->get_name())){
             return f;
         }
    }
    return nullptr;
}

std::string osd_object_database::make_unique_font_name(std::string const & name_in)const
{
   std::string name_out = name_in;
   int val = 1;
   for(;;){
      bool name_unique = true;
      // search through font looking for name
      for( auto iter : m_font_map){
         db_font* f = iter.second;
         assert((f != nullptr ) && __LINE__);
         if(name_out == f->get_name()){
            name_unique = false;
            // modify the output name by prepending "copy_n_name
            char* const buf = static_cast<char* const>(malloc( name_in.length() + 2 + 30));
            sprintf(buf,"copy_%d_%s",val,name_in.c_str());
            name_out = buf;
            free(buf);
            ++val;
            break;
         }
      }
      if ( name_unique == true){
         return name_out;
      }else{
         continue;
      }
   }
   // shouldnt get here
   assert(false && __LINE__);
   return "";
}

std::string osd_object_database::make_unique_bitmap_name(std::string const & name_in)const
{
   std::string name_out = name_in;
   int val = 1;
   for(;;){
      bool name_unique = true;
      // search through bitmaps looking for name
      for( auto handle : m_bitmaps){
         assert(( handle != -1) && __LINE__);
         auto image = find_bitmap_by_handle(handle);
         assert((image != nullptr ) && __LINE__);
         auto bmp = dynamic_cast<dynamic_bitmap*>( image);
         assert( bmp && __LINE__);
         if(name_out == bmp->get_name()){
            name_unique = false;
            // modify the output name by prepending "copy_n_name
            char* const buf = static_cast<char* const>(malloc( name_in.length() + 2 + 30));
            sprintf(buf,"copy_%d_%s",val,name_in.c_str());
            name_out = buf;
            free(buf);
            ++val;
            break;
         }
      }
      if ( name_unique == true){
         return name_out;
      }else{
         continue;
      }
   }
   // shouldnt get here
   assert(false && __LINE__);
   return "";
}

// image was created on heap or by clone
// resource container takes ownership
void osd_object_database::set_image_handle(int handle, dynamic_bitmap* image)
{
      assert(( handle != -1) && __LINE__);
      assert(( image != nullptr) && __LINE__);
      dynamic_bitmap* old_image = find_bitmap_by_handle(handle);
      assert((old_image != nullptr) && __LINE__);

      old_image->destroy();
      m_osd_image_map.at(handle) = image;

}

int osd_object_database::get_new_handle()
{
   for (int i = m_min_handle; i < m_max_handle; ++i) {
         auto iter = m_handles_in_use.find (i);
         if (iter == m_handles_in_use.end()) {
               m_handles_in_use.insert (i);
               return i;
            }
   }
   m_handles_in_use.insert(m_max_handle);
   int result = m_max_handle;
   ++m_max_handle;
   return result;
}

// can only free bitmaps. to remove fonts do all at one go
bool osd_object_database::free_handle (int handle)
{
   auto iter = m_handles_in_use.find (handle);
   if (iter != m_handles_in_use.end()) {
         m_handles_in_use.erase (iter);
         return true;
      }
   else {
         return false;
      }
}

// can find bitmaps and font elements
osd_object_database::dynamic_bitmap*
osd_object_database::find_bitmap_by_handle(int handle)const
{
   auto iter = m_osd_image_map.find(handle);
   if(iter != m_osd_image_map.end()){
      return iter->second;
   }else{
      return nullptr;
   }
}

db_font*
osd_object_database::find_font_by_handle(int handle)const
{
   auto iter = m_font_map.find(handle);
   if(iter != m_font_map.end()){
      return iter->second;
   }else{
      return nullptr;
   }
}

// after iterating and moving do clean_bitmap_handles() to remove dead handles
osd_object_database::dynamic_bitmap*
osd_object_database::move_bitmap_by_handle(int handle)
{
   assert(handle != -1);
   auto iter = m_osd_image_map.find(handle);
   if(iter == m_osd_image_map.end()){
      return nullptr;
   };
   dynamic_bitmap* bmp = iter->second;
   m_osd_image_map.erase(iter);

   assert(bmp && __LINE__);
   bool handle_invalidated = false;
   for ( auto iter = m_bitmaps.begin(), end = m_bitmaps.end(); iter != end; ++iter){
      if (*iter == handle){
         *iter = -1;
         handle_invalidated = true;
         break;
      }
   }
   assert(handle_invalidated && __LINE__);
   assert(free_handle(handle) && __LINE__);
   return bmp;
}

// removes font element handle from db 
// but element not in bitmaps vector
osd_object_database::dynamic_bitmap*
osd_object_database::move_font_element_by_handle(int handle)
{
   assert(handle != -1);
   auto iter = m_osd_image_map.find(handle);
   if(iter == m_osd_image_map.end()){
      return nullptr;
   };
   dynamic_bitmap* bmp = iter->second;
   assert(bmp && __LINE__);
   m_osd_image_map.erase(iter);
   assert(free_handle(handle) && __LINE__);
   return bmp;
}

db_font* osd_object_database::move_font_by_handle(int handle)
{
   assert(handle != -1);
   auto iter = m_font_map.find(handle);
   if(iter == m_font_map.end()){
      return nullptr;
   };
   db_font* cur_font = iter->second;
   m_font_map.erase(iter);

   assert(cur_font && __LINE__);
   bool handle_invalidated = false;
   for ( auto iter = m_fonts.begin(), end = m_fonts.end(); iter != end; ++iter){
      if (*iter == handle){
         *iter = -1;
         handle_invalidated = true;
         break;
      }
   }
   assert(handle_invalidated && __LINE__);
   assert(free_handle(handle) && __LINE__);
   return cur_font;
}

bool osd_object_database::clean_bitmap_handles()
{
  std::vector<int> temp;
   for ( auto v : m_bitmaps){
      if ( v != -1){
         temp.push_back(v);
      }
   }
   m_bitmaps = temp;
   return true;
}
// can only add bitmaps not font elements
int osd_object_database::add_bitmap( dynamic_bitmap* bmp)
{
   int new_handle = get_new_handle();
   m_osd_image_map.insert({new_handle,bmp});
   m_bitmaps.push_back(new_handle);
   return new_handle;
}

// assume its already part of the font
int osd_object_database::add_font_element( dynamic_bitmap* bmp)
{
   int new_handle = get_new_handle();
   m_osd_image_map.insert({new_handle,bmp});
   return new_handle;
}

int osd_object_database::add_font( db_font* f)
{
   int new_handle = get_new_handle();
   m_font_map.insert({new_handle,f});
   m_fonts.push_back(new_handle);
   return new_handle;
}
