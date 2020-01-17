#include "board.h"
#include "random_agent.hpp"
//#include"var.h"

signed main()
{
  cout << "test board validity" << endl;
  Board b;
  b.init();
  cout << b << endl;
  RandomAgent p1(BLACK);
  RandomAgent p2(WHITE);
  int i = 0;
  while(b.status() == NOTEND)
  {
    if(b.player_to_go() == BLACK)
    {
      Action a = p1.genmove(b);
      b.take_action(a);
      cout << ++i << "th action" << endl;
      cout << a;
    }
    else if(b.player_to_go() == WHITE)
    {
      Action a = p2.genmove(b);
      b.take_action(a);
      cout << ++i << "th action" << endl;
      cout << a;
    }
    cout << b << endl;
    //cout << b.valid_actions_mask(b.player_to_go());
  }
  int bscore = b.calc_territory().first;
  int wscore = b.calc_territory().second;
  debug(bscore);
  debug(wscore);
  b.print_air();
  /*
  b.print_contact();

  cout << "test contact points" << endl;
  rep(i, 0, 4)
  {
    //infinite loop
    //debug(b.contactEdges(initial_contact_points[i]));
    cout << "contact edges for cc Pos " << initial_contact_points[i].x << ' ' << initial_contact_points[i].y << endl;
    cout << b.contactEdges(initial_contact_points[i]) << endl;
  }
  */
  return 0;
}

/*
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
*/
