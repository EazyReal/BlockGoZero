#include "board.h"
#include<random>
#include<ctime>

using namespace std;
using namespace __board_namespace__;

class RandomAgent
{
public:
  Color id;
  int seed;

public:
  RandomAgent();
  RandomAgent(Color id);
  RandomAgent(Color id, int seed);
  //~RandomAgent();
  Action genmove(Board& b);
};

RandomAgent::RandomAgent()
{
  this->id = BLACK;
  this->seed = time(NULL);
  srand(seed);
}

RandomAgent::RandomAgent(Color id)
{
  this->id = id;
  this->seed = time(NULL);
  srand(seed);
}

RandomAgent::RandomAgent(Color id, int seed)
{
  this->id = id;
  this->seed = seed;
  srand(seed);
}

Action RandomAgent::genmove(Board& b)
{
  vector<Action> actions = b.valid_actions(id);
  return actions[rand()%actions.size()];
}
