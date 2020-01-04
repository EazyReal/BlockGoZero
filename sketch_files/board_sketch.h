#include <cstdio>
#include <block.h>

class Action
{
  BLOCK block;
  POS pos;

  bool is_valid();
};

enum STATUS {
	NOTEND = -1,
	BLACKWIN	= 0,
  WHITEWIN = 1
};

class Board
{
  const size_t NA; //number of actions boardsize*blocks(with turns and mirrors)
  //use cnt of 7 type of block to control can use or not
  BLOCK blocks[NA];
  //take action player, block, pos
  //use data base of positions instead?
  void init();
  STATUS take_action(COLOR player,ACTION action);
  //valid action given current board
  vector<ACTION> valid_actions();
  //return status of the
  STATUS status(); //which player to play, or winner
  //bool is_end();
  void calc_territory(int& , int&);
  //return valid action ids (client/player side function)

client
  bitset<13*13*NA> valid_actions();
  //
};

vector<ACTION> all_actions();
