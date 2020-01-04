#include "board.h"
//#include"var.h"


signed main()
{
  Board b;
  b.init();
  //cout << blocks_locs[0][0].x <<  ' ' << blocks_locs[0][0].y << endl;
  //b.take_action(Action(0, 19, {3,3}));
  //b.take_action(Action(1, 19, {3,9}));
  auto v = b.valid_actions(1);
  cout << "there are " << v.size() << "valid actions:" << endl;
  for(auto a : v)
  {
    Board b2;
    b2.init();
    b2.take_action(a);
    cout << b2 << endl;
  }
  cout << b;
  cout << "test validity" << endl;
  return 0;
}
