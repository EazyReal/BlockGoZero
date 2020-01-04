#include <cstdio>
#include <vector>
#include <iostream>
#include <array>

using namespace std;

#define pb push_back
#define NTYPE 9
//4*4 is max
#define MAXN 4
#define N 4

typedef array<array<int,N>, N> BLOCKGRID; // of size N, care

ostream& operator<<(ostream& os, const BLOCKGRID& rhs)
{
  for(int i = 0; i < N; ++i, cout<<endl)for(int j = 0; j < N; ++j)
  cout << rhs[i][j];
  return os;
}


class POS
{
public:
  int x,y;
  POS(int _x, int _y) : x(_x), y(_y){}
  POS operator+(const POS&rhs)
  {
    return POS(x+rhs.x, y+rhs.y);
  }
};


void rotate90(BLOCKGRID& mat)
{
    for (int x = 0; x < N/2; x++)
    {
        for (int y = x; y < N-x-1; y++)
        {
            int temp = mat[x][y];
            mat[x][y] = mat[y][N-1-x];
            mat[y][N-1-x] = mat[N-1-x][N-1-y];
            mat[N-1-x][N-1-y] = mat[N-1-y][x];
            mat[N-1-y][x] = temp;
        }
    }
}

class BLOCK
{
public:
  //const int N = 4;
  BLOCKGRID grid;
  int id; //type id
  int n;
  vector<vector<vector<POS> > > prss; //rotate(4)*n*n-1;

  BLOCK(BLOCKGRID a)
  {
    for(int i = 0; i < N; ++i)for(int j = 0; j < N; ++j)
      this->grid[i][j] = a[i][j];
    for(int i = 0; i < 4; ++i)
    {
      vector<vector<POS> > pss;
      rotate90(grid);
      for(int pi = 0; pi < N; ++pi)for(int pj = 0; pj < N; ++pj)if(grid[pi][pj])
      {
        vector<POS> ps;
        for(int p2i = 0; p2i < N; ++p2i)for(int p2j = 0; p2j < N; ++p2j)if(grid[p2i][p2j])
        {
          ps.pb(POS(p2i-pi, p2j-pj));
        }
        pss.pb(ps);
      }
      prss.pb(pss);
    }
  }
};


void init_block_data()
{
  vector<BLOCKGRID> grids;
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {1,1,1,1} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,0,0,0},
  {1,1,1,0},
  {0,0,1,0} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,0,0,0},
  {0,1,1,1},
  {0,1,0,0} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,1,1,0},
  {0,1,1,0},
  {0,0,0,0} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,0,0,0},
  {0,1,1,0},
  {1,1,0,0} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,0,0,0},
  {0,1,0,0},
  {1,1,1,0} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,0,0,0},
  {0,1,1,0},
  {0,0,1,1} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,1,0,0},
  {0,0,0,0},
  {0,0,0,0} }});
  grids.push_back((BLOCKGRID){{
  {0,0,0,0},
  {0,1,0,0},
  {0,0,0,0},
  {0,0,0,0} }});

/*
  for(auto grid : grids)
  {
    cout << grid << endl;
  }
*/

  BLOCK b(grids[0]);
  for(auto pss: b.prss)
  {
    cout << "rotate90" << endl;
    for(auto ps: pss)
    {
      cout << "for a point" << endl;
      for(auto p: ps)
      {
        cout << "point " << p.x << " " << p.y <<  " " << endl;
      }
    }
  }
};

int main()
{
  init_block_data();
  return 0;
}
