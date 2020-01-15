#include "board.h"
//#include"var.h"

//using namespace std;

ostream& operator<<(ostream& os, const Board& rhs)
{
  for(int i = 0; i < BOARDN; ++i, cout<<endl)for(int j = 0; j < BOARDN; ++j)
  {
    if(rhs.b[i][j] == -1) cout << ". ";
    if(rhs.b[i][j] == 0) cout << "o ";
    if(rhs.b[i][j] == 1) cout << "x ";
  }
  return os;
}

ostream& operator<<(ostream& os, const Pos& rhs)
{
  cout << "POS: " << rhs.x << ' ' <<  rhs.y << endl;
  return os;
}

ostream& operator<<(ostream& os, const Action& rhs)
{
  cout << "Action: " << endl;
  cout << "player: " << rhs.player << endl;
  cout << "iso_id: " << rhs.iso_id << endl;
  Pos pos = rhs.origin-blocks_locs[rhs.iso_id][rhs.origin_id];
  cout << "origin: " <<  pos.x << " " << pos.y << endl;
  return os;
}

void Board::init()
{
  //init block cnt
  block_n[0] = initial_cnt;
  block_n[1] = initial_cnt;
  //init board with -1(empty)
  for(int i = 0; i < BOARDN; i++)
  {
    memset(b[i], -1, sizeof(b[i]));
    memset(contact_points[0][i], 0, sizeof(contact_points[0][i]));
    memset(contact_points[1][i], 0, sizeof(contact_points[1][i]));
  }
  //init contact_points
  for(const Pos& pos:initial_contact_points)
    contact_points[0][pos.x][pos.y] = contact_points[1][pos.x][pos.y] = 1;
  //init actions_taken
  actions_taken.clear();
  //iso2type => type2isos
  for(int i = 0; i < N_BLOCKTYPE; ++i) type2isos[i].clear();
  for(int i = 0; i < N_ISO; ++i) type2isos[iso2type[i]].push_back(i); //or written in .h file
  //allpos init
  for(int i = 0; i < BOARDVL; ++i) allpos[i] = Pos(i/BOARDN, i%BOARDN);
}

inline bool Board::inrange(const Pos& pos)
{
  return pos.x>=0 && pos.x<BOARDN && pos.y>=0 && pos.y<BOARDN;
}

//should use after check inrange
inline bool Board::occupied(const Pos& pos)
{
  //if(inrange(pos)) return true;
  return b[pos.x][pos.y] != -1;
}


bool Board::is_valid(const Action& action)
{
  if(block_n[action.player][iso2type[action.iso_id]] == 0) return false;
  bool contact = false;
  for(auto& dxy : blocks_locs[action.iso_id])
  {
    Pos loc = action.origin + dxy - blocks_locs[action.iso_id][action.origin_id];
    if(!inrange(loc) || occupied(loc)) return false;
    contact |= contact_points[action.player][loc.x][loc.y];
  }
  return contact;
}

Status Board::status()
{
  if(actions_taken.size() != 18) return NOTEND;
  else return BLACKWIN;
}

Status Board::take_action(Action action)
{
  assert(is_valid(action));
  for(auto& dxy : blocks_locs[action.iso_id])
  {
    Pos loc = action.origin + dxy - blocks_locs[action.iso_id][action.origin_id];
    this->b[loc.x][loc.y] = action.player;
  }
  //action related value changes
  block_n[action.player][iso2type[action.iso_id]]--;
  this->actions_taken.push_back(action);
  //contact_points changes
  for(auto& dxy : blocks_locs[action.iso_id])
  {
    Pos loc = action.origin + dxy - blocks_locs[action.iso_id][action.origin_id];
    for(const Pos &dxdyi : dxdy)
    {
      Pos mark_pos = loc+dxdyi;
      //debug contact, didnt test inrange of mark pos
      if(inrange(mark_pos))
        this->contact_points[action.player][mark_pos.x][mark_pos.y] = 1;
    }
  }
  return this->status();
}

vector<Action> Board::valid_actions(Color player)
{
  vector<Action> ret;
  if(actions_taken.size() == 18) return ret;
  for(Pos& origin: allpos)
  {
    if(occupied(origin) || !contact_points[player][origin.x][origin.y]) continue;
    for(int type_id = 0; type_id < N_BLOCKTYPE; ++type_id)
    {
      if(block_n[player][type_id] == 0) continue;
      for(int& iso_id : type2isos[type_id])
      {
        for(int origin_id = 0; origin_id < 4; ++origin_id)
        {
          //action connected that transform to noraml form(origin_id = 0);
          Action tmp(player, iso_id, origin-blocks_locs[iso_id][origin_id], 0);
          if(is_valid(tmp)) ret.push_back(tmp);
        }
      }
    }
  }
  return ret;
}


Color Board::player_to_go()
{
  return actions_taken.size()%2;
}

//debug contact, didnt test inrange of mark pos
void Board::print_contact()
{
  cout << "contact point matrix of player 0\n";
  for(int i = 0 ;i < BOARDN; ++i, cout << endl)
  for(int j = 0 ;j < BOARDN; ++j)
  if(occupied({i,j})) cout << ((b[i][j] == 0)?"o ":". ");
  else cout << contact_points[0][i][j] << ' ';

  cout << "contact point matrix of player 1\n";
  for(int i = 0 ;i < BOARDN; ++i, cout << endl)
  for(int j = 0 ;j < BOARDN; ++j)
  if(occupied({i,j})) cout << ((b[i][j] == 0)?"o ":". ");
  else cout << contact_points[1][i][j] << ' ';
  return;
}

template <typename T>
inline void Board::initBshapeArray(T arr[BOARDN][BOARDN], T val)
{
  rep(i, 0, BOARDN)rep(j, 0, BOARDN) arr[i][j] = val;
}

template <typename T>
inline void Board::printBshapeArray(T arr[BOARDN][BOARDN], string arr_name)
{
  cout << "this is array: " << arr_name << endl;
  for(int i = 0; i < BOARDN; ++i, cout << endl)rep(j, 0, BOARDN)
  {
    cout << arr[i][j] << ' ';
  }
}

/*
//no init visit inside
template <typename T>
void Board::BFScoloring(T arr[BOARDN][BOARDN], bool vis[BOARDN][BOARDN], Pos initpos, T val, T obs)
{
  queue<Pos> q;
  q.push(initpos);
  while(!q.empty)
  {

  }
}
*/

//return contact edges count
//my method bfs to find points contact with edge
//return edge-counting point-set's cc count
//notice that if u choose an empty site, initial color will be empty
int Board::contactEdges(Pos initpos)
{
  bool vis[BOARDN][BOARDN]; // new vis, should be release after function call
  bool mark[BOARDN][BOARDN]; //mark the contact points
  initBshapeArray(vis, false); //can only use false for template
  initBshapeArray(mark, false);
  queue<Pos> q;
  int initial_color = b[initpos.x][initpos.y];
  q.push(initpos);
  while(!q.empty())
  {
    Pos curPos = q.front(); q.pop();
    rep(i, 0, 4)
    {
      Pos newPos = curPos + __board_namespace__::dxdy[i];
      if(!inrange(newPos)) {mark[curPos.x][curPos.y] = 1; continue;} //mark contact point
      //push if new position is same color
      if(b[newPos.x][newPos.y] == initial_color && !vis[newPos.x][newPos.y])
      {
        q.push(newPos);
        vis[newPos.x][newPos.y] = true;
      }
    }
  }
  //here then mark is the points in contact with edges
  printBshapeArray(mark, GET_VAR_NAME(mark));
  //q should be empty
  auto isEdge = [](Pos pos)
  {
    return pos.x == BOARDN-1 || pos.y == BOARDN-1 || pos.x == 0 || pos.y == 0;
  };
  //color ccs return 1 if it is marked and not unmarked(new)
  auto subroutine = [&](Pos pos) mutable //pass all by referece, mutable
  {
    if(!mark[pos.x][pos.y]) return 0;
    //cc_cnt = cc_cnt + 1; cout << "hi debug" << endl;
    q.push(pos);
    while(!q.empty())
    {
      Pos curPos = q.front(); q.pop();
      mark[curPos.x][curPos.y] = 0;//implicitly set vis = false
      rep(i, 0, 4)
      {
        Pos newPos = curPos + __board_namespace__::dxdy[i];
        if(!this->inrange(newPos)||!isEdge(newPos)) continue; //out of range / out of edge area
        if(mark[newPos.x][newPos.y]) q.push(newPos);
      }
    }
    return 1;
  };
  int cc_cnt = 0;
  rep(i, 0, BOARDN)
  {
    cc_cnt += subroutine(Pos(0, i));
    cc_cnt += subroutine(Pos(i, 0));
    cc_cnt += subroutine(Pos(BOARDN-1, i));
    cc_cnt += subroutine(Pos(i, BOARDN-1));
  }
  return cc_cnt;
}

//calc_territory
pair<int, int> Board::calc_territory()
{
  int bscore = 0;
  int wscore = 0;
  //initBshapeArray(vis, false);

  return make_pair(bscore, wscore);
}

//test main
/*
signed main()
{
  Board b;
  b.init();
  //cout << blocks_locs[0][0].x <<  ' ' << blocks_locs[0][0].y << endl;
  cout << b;
  cout << "test validity" << endl;
  return 0;
}
*/
