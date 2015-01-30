
#include "../../document.hpp"
#include "../../osd_bmp_app.hpp"

#include "drawing.hpp"

drawing::vect drawing::get_size() const
{
  return wxGetApp().get_document()->get_page_size();
}
