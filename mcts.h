#ifndef MCTSH
#define MCTSH
#include"bitboard.h"
#include"boardcn.h"
#include"board.h"
#include"uctnode.h"
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
#define BLACK 0
#define WHITE 1

#define USEROUNDS //no define USETIME, from rave2.8, ucb1.4

#define BOARDSZ 9
#define BOARDVL 81
#define DEFAUT_TIME_PER_STEP (time_t(0.5*CLOCKS_PER_SEC))
#define DEFAUT_SIMS 50000
#define BLOCKSIZE 50

#define RESIGN 0.2

typedef int Action;//for clarity

using namespace std;

class MCTS
{
public:
  Action best_action(board &init_b, bool color, int simu_per_step = DEFAUT_SIMS);
  double calc_winrate();
  void init_with_board(board &b); //init MCTS with board
  void reset(board &b); //reset board and path before each cycle
  void clear(); //clear MCTS by delete root

  Node* select();
  bool roll_out();
  void backpropogation(bool res);

  int total;
  Node* root; //the current position to play, root of select
  board root_board; //root board should be saved
  board simu_board; //board for slect + simulation
  vector<Node*> path; //record path of each select
  vector<Action> rave_path[2]; //for amaf rave value update
  //heuristic rave value, not used in final version of ez nogo, can be extended(final to do)
  //double rave_cnt[2][BOARDVL]; //rave count, amaf, action taken = counted
  //double rave_num[2][BOARDVL]; //rave value, amaf, dont consider (S,A) only A
  //note that the way to add rava value is
  //run over selected path, update selected move's child's rave val once

  //rollout follows that both player try to capture both can play positions first
  //my idea is the same as hahanogo, so some of its board functions for rollout is used
  int bpos[BOARDVL+1], wpos[BOARDVL+1], bothpos[BOARDVL+1]; //for recoding possible move to play for rollout
  int bpos_sz, wpos_sz, bothpos_sz; //vector sz

  //btw beause of base num stablize the initial Q(s,a)m, thus unexplored child will always get accesed before win 1 child?
  //http://www.cs.utexas.edu/~pstone/Courses/394Rspring13/resources/mcrave.pdf simu_tree+simu_defaut is a good way to think
};


#endif //MCTSH
