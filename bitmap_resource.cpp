
#include "bitmap_resource.hpp"

bool bitmap_resource_t::get_bitmap_handle_at(size_t i, int & handle_out)const
{
   if ( i >= get_num_bitmaps()){
      return false;
   }
   int handle = m_bitmaps.at(i);
   assert((handle != -1) && __LINE__);
   handle_out = handle;
   return true;
}

bool bitmap_resource_t::find_bitmap_name(std::string const & name_in)const
{
    for( auto v : m_osd_image_map){
         osd_image* image = v.second;
         auto bmp = dynamic_cast<osd_bitmap*>( image);
         if(bmp && (name_in == bmp->get_name())){
             return true;
         }
    }
    return false;
}

std::string bitmap_resource_t::make_unique_bitmap_name(std::string const & name_in)const
{
   std::string name_out = name_in;
   int val = 1;
   for(;;){
      bool name_unique = true;
      // search through bitmaps looking for name
      for( auto v : m_osd_image_map){
         osd_image* image = v.second;
         auto bmp = dynamic_cast<osd_bitmap*>( image);
         if(bmp && (name_out == bmp->get_name())){
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
void bitmap_resource_t::set_image_handle(int handle, osd_image* image)
{
      assert(( handle != -1) && __LINE__);
      assert(( image != nullptr) && __LINE__);
      osd_image* old_image = find_osd_image(handle);
      assert((old_image != nullptr) && __LINE__);
      
      old_image->destroy();
      m_osd_image_map.at(handle) = image;
      
}
 
int bitmap_resource_t::get_new_handle()
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
bool bitmap_resource_t::free_handle (int handle)
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
osd_image* bitmap_resource_t::find_osd_image(int handle)const
{
   auto iter = m_osd_image_map.find(handle);
   if(iter != m_osd_image_map.end()){
      return iter->second;
   }else{
      return nullptr;
   }
}


// after iterating and moving do clean_bitmap_handles() to remove dead handles
// 
osd_bitmap* bitmap_resource_t::move_osd_bitmap(int handle)
{
   assert(handle != -1);
   auto iter = m_osd_image_map.find(handle);
   if(iter == m_osd_image_map.end()){
      return nullptr;
   };
   
   osd_image* image = iter->second;
   osd_bitmap* bmp = dynamic_cast<osd_bitmap*>(image);
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

bool bitmap_resource_t::clean_bitmap_handles()
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
int bitmap_resource_t::add_bitmap( osd_bitmap* bmp)
{
   int new_handle = get_new_handle();
   m_osd_image_map.insert({new_handle,bmp});
   m_bitmaps.push_back(new_handle);
   return new_handle;
}