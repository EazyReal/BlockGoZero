#ifndef __GLOBALVARS__
#define __GLOBALVARS__

#include <array>
#include <vector>

//should define array<array<T, BOARDN>, BOARDN>

//should not do calcaulation in rep
#define rep(i, st, n) for (int i = (st); i < (n); ++i)
#define debug(x) std::cout << #x << ": " << x << endl
#define GET_VAR_NAME(x) (#x)

using namespace std;

typedef bool Color;

namespace __board_namespace__{
  enum Status {
  	NOTEND = -1,
  	BLACKWIN	= 0,
    WHITEWIN = 1,
    TIE = 2
  };

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
    Pos operator=(const Pos&rhs)
    {
      this->x = rhs.x;
      this->y = rhs.y;
      return *this; //warning
    }
    friend ostream& operator<<(ostream& os, const Pos& pos);
  };

  static const size_t BOARDN = 13;
  static const size_t BOARDVL = 169;
  //number of type of blocks
  static const size_t N_BLOCKTYPE = 8;
  //number of iso of blocks
  static const size_t N_ISO = 20;
  static const size_t N_ACTION = BOARDVL*N_ISO;
  //color
  static const bool BLACK = 0;
  static const bool WHITE = 1;

  //static const arrays
  static array<Pos, BOARDVL> allpos = {};
  //iso id to type id
  static const array<int, N_ISO> iso2type = {0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7};
  static array<vector<int>, N_BLOCKTYPE> type2isos = {};
  //usable block num;
  static const array<int, N_BLOCKTYPE> initial_cnt = {2, 1, 1, 1, 1, 1, 1, 1};
  //dxdy
  static const array<Pos, 4> dxdy =
  {{
    {0,1}, {1,0}, {0,-1}, {-1,0}
  }};
  static const array<Pos, 4> initial_contact_points =
  {{
    {3,3}, {3,9}, {9,3}, {9,9}
  }};
  static const array<array<Pos, 4>, N_ISO> blocks_locs =
  //int blocks[2][4][2] =
  {{//dx,dy
  //.
    {{ {0, 0}, {0, 0}, {0, 0}, {0, 0} }},
  //__
  //__
    {{ {0, 0}, {0, 1}, {1, 0}, {1, 1} }},
  //____
    {{ {0, 0}, {0, 1}, {0, 2}, {0, 3} }},
    {{ {0, 0}, {1, 0}, {2, 0}, {3, 0} }},
  //__
  //  |__
    {{ {0, 0}, {1, 0}, {1, -1}, {2, -1} }},
    {{ {0, 0}, {0, 1}, {1, 1}, {1, 2} }},
  //  __
  //__|
    {{ {0, 0}, {1, 0}, {1, 1}, {2, 1} }},
    {{ {0, 0}, {0, 1}, {-1, 1}, {-1, 2} }},
  //___
  //|
    {{ {0, 0}, {0, 1}, {1, 1}, {2, 1} }},
    {{ {0, 0}, {1, 0}, {1, -1}, {1, -2} }},
    {{ {0, 0}, {1, 0}, {2, 0}, {2, 1} }},
    {{ {0, 0}, {1, 0}, {0, 1}, {0, 2} }},
  //___
  //  |
    {{ {0, 0}, {1, 0}, {2, 0}, {2, -1} }},
    {{ {0, 0}, {1, 0}, {1, 1}, {1, 2} }},
    {{ {0, 0}, {0, 1}, {1, 0}, {2, 0} }},
    {{ {0, 0}, {0, 1}, {0, 2}, {1, 2} }},
  // |
  //___
    {{ {0, 0}, {1, 0}, {2, 0}, {1, 1} }},
    {{ {0, 0}, {0, 1}, {0, 2}, {1, 1} }},
    {{ {0, 0}, {0, 1}, {-1, 1}, {1, 1} }},
    {{ {0, 0}, {0, 1}, {0, 2}, {-1, 1} }}
  }};


}


/*
#define BOARDN 13
#define BOARDVL 169
#define N_BLOCKTYPE 8
#define N_ISO 20
#define N_ACTION 20
#define BLACK 0
#define WHITE 1
*/



//all poss



#endif
