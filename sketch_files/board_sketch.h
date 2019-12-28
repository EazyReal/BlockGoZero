#include <cstdio>

class Board
{
  const size_t NA; //number of actions boardsize*blocks(with turns and mirrors)
  //use cnt of 7 type of block to control can use or not
  Block blocks[NA];
  //take action player, block, pos
  void init();
  int take_action(player, block, pos);
  //
  STATUS status(); //which player to play, or winner
  bool is_end();
  void calc_territory(int& , int&);
  //return valid action ids (client/player side function)

client
  bitset<NA> valid_moves();
  //
};
