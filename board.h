#ifndef __BOARDH__
#define __BOARDH__
#include<iostream>
#include<map>
#include<string.h>
#include<vector>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<math.h>
#include<array>
#include<bitset>
#include<initializer_list>
#include<cassert>
#include<string>
#include<cstring>
#include"var.h"
//#include<Windows.h>
//#include "block.h"

using namespace std;
using namespace __board_namespace__;
//using namespace __board_namespace__;
//namespace can be used for block related
typedef bool Color;

/*
class Pos
{
public:
  int x,y;
  Pos() : x(0), y(0) {}
  Pos(int _x, int _y) : x(_x), y(_y){}
  Pos(initializer_list<int> a) : x(*a.begin()), y(*(a.begin()+1)){}
  Pos operator+(const Pos&rhs) const
  {
    return Pos(this->x+rhs.x, this->y+rhs.y);
  }
  Pos operator-(const Pos&rhs) const
  {
    return Pos(this->x-rhs.x, this->y-rhs.y);
  }
};
*/

class Action
{
public:
  Color player;
  int iso_id;
  Pos origin;
  int origin_id;

  Action(bool _player, int _iso_id, Pos _origin, int _origin_id = 0) :
    player(_player), iso_id(_iso_id), origin(_origin), origin_id(_origin_id){}
  Action(initializer_list<int> a)
  {
    player = *a.begin();
    iso_id = *(a.begin()+1);
    origin = Pos(*(a.begin()+2), *(a.begin()+3));
    origin_id = *(a.begin()+4);
  }
  friend ostream& operator<<(ostream& os, const Action& action);
};

enum Status {
	NOTEND = -1,
	BLACKWIN	= 0,
  WHITEWIN = 1
};

class Board
{
public:
  //board related variants
  int b[BOARDN][BOARDN]; //board
  bool contact_points[2][BOARDN][BOARDN]; //should check emptyness, can be occupied
  array<int, N_BLOCKTYPE> block_n[2]; //block number left
  //log of play
  vector<Action> actions_taken;
  //init Positions
  void init();
  //take action player, block, Pos
  Status take_action(Action action);
  //valid action given current board
  vector<Action> valid_actions(Color player);
  //return status of the
  Status status(); //which player to play, or winner
  //bool is_end();
  void calc_territory(int& , int&);

  bool inrange(const Pos& pos);
  bool occupied(const Pos& pos);
  bool is_valid(const Action& action);

  Board(){};
  //overload <<
  friend ostream& operator<<(ostream& os, const Board& board);
//client:
//return valid action ids (client/player side function)
//  bitset<NA> valid_actions();
  //
};

//ostream& operator<<(ostream& os, const Board& rhs);


#endif
