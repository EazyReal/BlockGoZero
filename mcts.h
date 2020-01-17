#ifndef MCTSH
#define MCTSH
#include<iostream>
#include<map>
#include<string.h>
#include<vector>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<random> //new random lib c++14
#include<math.h>
#include<iomanip>
#include <ctime>

#include"board.h"

/*
typedef array<float, N_ACTION> Policy;

class Model
{
public:
  Policy getP(Board& b);
};

Policy Model::getP(Board& b) //return random policy
{
  bitset<N_ACTION> bs =  b.valid_actions_mask(b.player_to_go());
  double p = 1.0/bs.count();
  Policy ret;
  rep(i, 0, N_ACTION) if(bs.test(i)) ret[i] = p;
  return ret;
}

class MCTS
{
public:
  Action best_action(Board &init_b, Color color, int simu_per_step = DEFAUT_SIMS);
  double calc_winrate();
  void init_with_board(Board &b); //init MCTS with board
  void reset(Board &b); //reset board and path before each cycle
  void clear(); //clear MCTS by delete root

  Node* select();
  bool roll_out();
  void backpropogation(bool res);

  Model model; //tmp, change to network later
  int total;
  Node* root;
  board root_board;
  board simu_board;
  vector<Node*> path;
};

*/
#endif //MCTSH
