#include "uctnode.h"

Node::Node(){};

Node::~Node()
{
  for(int i = 0; i < cptr.size(); i++)
  {
    delete cptr[i];
  }
  cptr.clear();
};

inline double Node::getQ()
{
  //4.0 up no INFQ
//  if(cnt == BASENUM) return INFQ
  //assert(parent != nullptr);
#ifndef USERAVEQ
  return (num/cnt)+UCB_C*sqrt(parent->log_cnt/cnt);
#endif
  //note that: this is for b ~= 0 in the fomulla of beta = N'/(N+N'+4N*N'*b*b) when b is small
  assert(rcnt+cnt != 0);
  return (num+rnum+UCB_C*sqrt(parent->log_cnt*cnt))/(rcnt+cnt);
}

//
//ver 4.0 up => for tuning
//will update most_visited_child as well
vector<double> Node::getPi()
{
	vector<double> pi(BOARDSSIZE, 0);
  if(cptr.size() == 0) { mvc = nullptr; return pi; }
	double sum = 0.0;
  double maxv = -1.0;
	for (int i = 0; i < cptr.size(); i++)
  {
    sum += cptr[i]->cnt;
    if(cptr[i]->cnt > maxv) {maxv = cptr[i]->cnt; mvc = cptr[i];}
    //7.0 try num instead of cnt(wincount first)
    //if(cptr[i]->num > maxv) {maxv = cptr[i]->num; mvc = cptr[i];}
  }
	for (int i = 0; i < cptr.size(); i++) pi[cptr[i]->pos] = cptr[i]->cnt / sum;
	return pi;
}

Node* Node::best_child()
{
  if(cptr.empty()) return nullptr; //segment fault prevantion

  vector<Node*> maxC;
  double maxQ = cptr[0]->getQ();
  maxC.push_back(cptr[0]);
  const double eps = 0.0001; //to deter if the value is close enough(to try both) in double

  for(int i = 1;i < cptr.size(); i++) //i = 1
  {
    double curQ = cptr[i]->getQ();
    double delta = curQ - maxQ;
    if(delta > eps)
    {
      maxQ = curQ;
      maxC.clear();
      maxC.push_back(cptr[i]);
    }else if(delta > -eps){
      maxC.push_back(cptr[i]);
    }
  }
  return maxC[rand()%maxC.size()];
}

void Node::update(bool result)
{
  if(result == color) num++;
  cnt++;
  //assert(cnt != 0);
  log_cnt = log(cnt);
}

void Node::rave_update(bool result)
{
  if(result == color) rnum++;
  rcnt++;
}

int Node::expand(board& b)
{
  //cptr.clear();
  bool c = !b.just_play_color(); //get player color
  for(int p = 0; p < BOARDVL; p++)if(b.check(p, c))
  {
    idx[p] = cptr.size();
    //this->show_stats();
    Node *ptr =  new Node;
    ptr->initNode(this, p, c);
    cptr.push_back(ptr);
  }
  //this->show_stats();
  return cptr.size(); // may use this
}

void Node::initNode(Node* p, Action i, bool c)
{
  this->parent = p; //parent = parent => bugged
  this-> mvc = nullptr;
  memset(idx, -1, sizeof(idx));
  this->color = c;
  this->pos = i;
  num = BASENUM/2.0;
  cnt = BASENUM; //stablize the beginnnig of training //but may cuz UCB problem
  //todo:r heuristic
  rnum = RBASENUM/2.0;
  rcnt = RBASENUM;
  log_cnt = 1; //may cuz UCB proble, care log(0)
}

double Node::show_stats()
{
  cout << "the node is after state of " << this->color  << ' ' << this->pos << endl;
  if(mvc == nullptr){
    cout << "most visited child is nullptr " << endl;
    return -1.0;
  }
  double winrate = mvc->num/mvc->cnt;
  double rval = mvc->rnum/mvc->rcnt;
  //cerr << "Node of color " << color << " pos " << pos << endl;
  cerr << "most visited child is " << mvc->color << ' ' << mvc->pos <<endl;
  cerr << "winrate of mvc is " << winrate << endl ;
  cerr << "rave of mvc is " << rval << endl;
  return winrate;
}
