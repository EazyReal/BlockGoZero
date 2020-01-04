#include <board_sketch.h>

bool ACTION::is_valid()
{
  if(block.USED) return false:
  for(auto pos : block.locations)
  {
    if(!valid(pos)) return false;
    if(!empty(pos)) return false;
  }
  return true;
}

STATUS status()
{
  for(auto Block )
}
