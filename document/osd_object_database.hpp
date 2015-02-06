#ifndef OSD_BITMAP_MAKER_BITMAP_RESOURCE_HPP_INCLUDED
#define OSD_BITMAP_MAKER_BITMAP_RESOURCE_HPP_INCLUDED

#include <cstdint>
#include <map>
#include <set>
#include <vector>

#include <quan/uav/osd/dynamic/object_database.hpp>
#include "../graphics_api/objects/db_font.hpp"

  struct osd_object_database : quan::uav::osd::dynamic::object_database{
      
      typedef quan::uav::osd::dynamic::bitmap dynamic_bitmap;
      typedef quan::uav::osd::dynamic::font dynamic_font;
      
      osd_object_database(): m_max_handle{m_min_handle}{}
      ~osd_object_database(){
            for ( auto iter : m_osd_image_map){
               iter.second->destroy();
            }
            for ( auto iter : m_font_map){
                delete iter.second;
            }
      }
      size_t get_num_fonts() const{ return m_fonts.size();}
      size_t get_num_bitmaps() const{ return m_bitmaps.size();}

      quan::uav::osd::dynamic::bitmap * get_bitmap_at(size_t i) const;
      quan::uav::osd::dynamic::font * get_font_at(size_t i) const;
      //db_font * get_font_at(size_t i);
       // for iterating through bitmap_handles
      bool get_bitmap_handle_at(size_t i, int & handle_out) const;
      bool get_font_handle_at(size_t i, int & handle_out) const;
      dynamic_bitmap* find_bitmap_by_handle( int handle)const;
      db_font* find_font_by_handle( int handle)const;

      // relinquish ownership of image and free its handle 
      // note that handles are replaced with invalid handles
      // use clean_bitmap_handles after iterating
      dynamic_bitmap* move_bitmap_by_handle(int handle);
      dynamic_bitmap* move_font_element_by_handle(int handle);
      db_font * move_font_by_handle(int handle);

      int add_bitmap( dynamic_bitmap*);
      int add_font_element( dynamic_bitmap*);
      int add_font(db_font*);
      void set_image_handle(int handle, dynamic_bitmap* image);
      dynamic_bitmap* find_bitmap_by_name(std::string const & name_in)const;
      db_font* find_font_by_name(std::string const & name_in)const;
      
      std::string make_unique_bitmap_name(std::string const & name_in)const;
      std::string make_unique_font_name(std::string const & name_in)const;
      bool clean_bitmap_handles();
    private:
      // only bitmap handles
      std::vector<int> m_bitmaps;
      std::vector<int> m_fonts;
      osd_object_database (osd_object_database const&) = delete;
      osd_object_database& operator= (osd_object_database const&) = delete;
      // holds db_font elements and bitmpas
      std::map<int,dynamic_bitmap*> m_osd_image_map;
      std::map<int, db_font*> m_font_map;
      std::set<int> m_handles_in_use;
      int get_new_handle();
      bool free_handle(int n);
      int m_max_handle;
      static constexpr int m_min_handle = 1;
  };

#endif // OSD_BITMAP_MAKER_BITMAP_RESOURCE_HPP_INCLUDED
