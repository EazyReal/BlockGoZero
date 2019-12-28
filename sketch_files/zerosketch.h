#include "mcts.h"
#include "pytorch.h" //or something

typedef vector<CASE, cmp> DATA;
//typedef priority_queue<DATA> DATAPQ; (sould use sample)
//class DATASET to manipulate

#define N_EPOCH 10
#define N_ITER 10
#define THRESHOULD 0.55

class ZeroTrainer
{
  DATA get_data(MODEL model); //will use MCTS, pytorch c++
  MODEL train(MODEL model, DATA data);
  double cmp(MODEL A, MODEL B); //return winrate
  //MODEL A=B; //assign or referece and despose old model
  void go();
};

class MODEL
{

}

class DATASET{
  void extend(DATASET rhs);
  DATA sample();
}

//c++ vector extend
/*
// reserve() is optional - just to improve performance
v.reserve(v.size() + distance(v_prime.begin(),v_prime.end()));
v.insert(v.end(),v_prime.begin(),v_prime.end());
*/

MODEL ZeroTrainer::go(int N_ITER, int N_EPOCH, double THRESHOULD)
{
  //init model cur_best_model;
  MODEL cur_best_model = MODEL(hyperparams);
  DATA datapool;
  //if use alphago pattern(55% threshould), zero version no iteration
  for(int i_it = 0; i_it < N_ITER; i_it++)
  {
    for(int i_ep = 0; i_ep < N_EPOCH; i_ep++) //this can be parallelized
    {
      DATA new_data = get_data(cur_best_model);
      datapool.extend(new_data);
    }
    MODEL new_model = train(cur_best_model, datapool);
    double winrate = cmp(new_model, cur_best_model);
    if(winrate > THRESHOULD):
      cur_best_model = new_model; //or reference
  }
  return cur_best_model;
}

MCTS_NET;
