#ifndef MAPS
#define MAPS

#include <vector>
using namespace std;

vector <vector<vector<int>>> maps{
    vector<vector<int>>{
    vector<int>{0,0,0,0,0,0,1,0,0,1},
    vector<int>{0,3,3,3,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,1,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,4,4,4,4,0,2,2,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,2,0,0,3,3,3,0},
    vector<int>{0,0,0,2,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,1,0,2,2,0}
  },

    vector<vector<int>> {
    vector<int>{0,0,2,0,0,2,0,0,0,0},
    vector<int>{0,0,2,0,0,2,0,0,1,0},
    vector<int>{1,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,1},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,4,4,4,4,0,0,0},
    vector<int>{3,0,0,0,0,0,0,0,0,0},
    vector<int>{3,0,3,3,3,0,0,0,2,0},
    vector<int>{3,0,0,0,0,0,1,0,2,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0}
  },

    vector<vector<int>> {
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{2,0,0,0,0,0,0,0,0,0},
    vector<int>{2,0,0,3,0,0,0,1,0,1},
    vector<int>{0,0,0,3,0,0,0,0,0,0},
    vector<int>{0,1,0,3,0,0,0,0,0,3},
    vector<int>{0,0,0,0,0,0,2,0,0,3},
    vector<int>{0,0,4,0,0,0,2,0,0,3},
    vector<int>{2,0,4,0,0,0,0,0,0,0},
    vector<int>{2,0,4,0,0,1,0,0,0,0},
    vector<int>{0,0,4,0,0,0,0,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,4,0,0,0,0,0,0,0},
    vector<int>{0,0,4,0,1,0,2,2,0,0},
    vector<int>{0,0,4,0,0,0,0,0,0,3},
    vector<int>{0,0,4,0,0,0,0,0,0,3},
    vector<int>{0,0,0,0,0,1,0,0,0,3},
    vector<int>{2,0,0,0,0,0,0,0,0,0},
    vector<int>{2,0,1,0,3,0,0,2,2,0},
    vector<int>{0,0,0,0,3,0,0,0,0,0},
    vector<int>{0,0,0,0,3,0,0,0,1,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,0,2,0,0,0,0,0},
    vector<int>{0,0,1,0,2,0,0,3,0,0},
    vector<int>{0,0,0,0,0,0,0,3,0,0},
    vector<int>{0,0,0,0,0,0,0,3,0,0},
    vector<int>{1,0,0,1,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,1,0,0,4,4,4,4,0,2},
    vector<int>{0,0,0,0,0,0,0,0,0,2},
    vector<int>{0,0,0,0,0,2,2,0,0,0},
    vector<int>{3,3,3,0,0,0,0,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,3,3,3,0,0,0,0,0},
    vector<int>{1,0,0,0,0,0,2,0,0,0},
    vector<int>{0,0,0,0,0,0,2,0,1,0},
    vector<int>{0,4,4,4,4,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{3,0,0,0,0,0,2,2,0,0},
    vector<int>{3,0,1,0,0,0,0,0,0,0},
    vector<int>{3,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,2,0,0,0,0,0,1},
    vector<int>{0,0,0,2,0,0,0,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,0,4,4,4,4,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,1,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,1,0,3,3,3},
    vector<int>{0,2,0,0,0,0,0,0,0,0},
    vector<int>{0,2,0,0,0,2,2,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,3,0,1,0,0,0,0,0,0},
    vector<int>{0,3,0,0,0,0,0,0,0,0},
    vector<int>{0,3,0,0,0,1,0,2,2,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,0,0,0,1,0,0,0},
    vector<int>{2,0,0,0,0,0,0,0,0,3},
    vector<int>{2,0,0,0,3,3,3,0,0,3},
    vector<int>{0,0,0,0,0,0,0,0,0,3},
    vector<int>{0,0,1,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,1,0,0},
    vector<int>{0,0,4,4,4,4,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,1},
    vector<int>{0,0,0,0,0,0,2,0,0,0},
    vector<int>{2,2,0,0,0,0,2,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,3,0,1,0,2,2,0},
    vector<int>{0,0,0,3,0,0,0,0,0,0},
    vector<int>{0,0,0,3,0,0,3,0,0,0},
    vector<int>{0,1,0,0,0,0,3,0,0,0},
    vector<int>{0,0,0,0,0,0,3,0,0,0},
    vector<int>{0,0,0,0,4,0,0,0,0,0},
    vector<int>{2,0,1,0,4,0,0,1,0,0},
    vector<int>{2,0,0,0,4,0,0,0,0,0},
    vector<int>{0,0,0,0,4,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,2,2,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,0,0,2,0,0,2,2},
    vector<int>{2,0,0,0,0,2,0,0,0,0},
    vector<int>{2,0,0,0,0,0,0,3,3,3},
    vector<int>{0,0,0,0,1,0,0,0,0,0},
    vector<int>{0,4,0,0,0,0,0,0,0,0},
    vector<int>{0,4,0,0,0,1,0,0,0,3},
    vector<int>{0,4,0,0,0,0,0,0,0,3},
    vector<int>{0,4,0,0,0,0,0,0,0,3},
    vector<int>{0,0,0,0,1,0,0,0,0,0},
    vector<int>{1,0,0,0,0,0,0,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{3,3,3,0,0,0,0,0,2,2},
    vector<int>{0,0,0,0,1,0,0,0,0,0},
    vector<int>{0,0,2,0,0,0,0,1,0,0},
    vector<int>{0,0,2,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,4,0,0,0,1,0,1,0},
    vector<int>{0,0,4,0,3,0,0,0,0,0},
    vector<int>{0,0,4,0,3,0,0,0,0,0},
    vector<int>{0,0,4,0,3,0,0,2,2,0}
  },

vector<vector<int>> {
    vector<int>{1,0,0,0,3,3,3,0,2,2},
    vector<int>{0,0,1,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,2,2,0,0,1},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,4,4,4,4,0,0},
    vector<int>{0,0,1,0,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,2,2,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,0},
    vector<int>{0,0,3,3,3,0,0,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,0,0,0,0,3,3,3},
    vector<int>{0,0,0,2,2,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,1,0},
    vector<int>{0,0,0,1,0,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,2,2,0,0},
    vector<int>{0,1,0,0,0,0,0,0,0,2},
    vector<int>{0,0,0,0,0,0,4,0,0,2},
    vector<int>{0,3,0,0,1,0,4,0,0,0},
    vector<int>{0,3,0,0,0,0,4,0,0,0},
    vector<int>{0,3,0,0,0,0,4,0,0,0}
  },

vector<vector<int>> {
    vector<int>{0,0,1,0,3,0,0,0,0,0},
    vector<int>{0,0,0,0,3,0,0,0,0,0},
    vector<int>{0,0,0,0,3,0,0,0,2,0},
    vector<int>{0,2,0,0,0,0,1,0,2,0},
    vector<int>{0,2,0,0,4,0,0,0,0,0},
    vector<int>{0,0,0,0,4,0,0,0,0,0},
    vector<int>{0,1,0,0,4,0,0,0,0,0},
    vector<int>{0,0,0,0,4,0,0,0,3,0},
    vector<int>{0,0,0,0,0,0,2,0,3,0},
    vector<int>{1,0,0,0,0,0,2,0,3,0}
  },

vector<vector<int>> {
    vector<int>{0,0,0,0,0,0,0,0,0,2},
    vector<int>{3,0,0,0,0,4,0,0,0,2},
    vector<int>{3,0,0,0,0,4,0,0,0,0},
    vector<int>{3,0,0,0,0,4,0,0,1,0},
    vector<int>{0,0,1,0,0,4,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,1,0,0},
    vector<int>{0,0,3,3,3,0,0,0,0,0},
    vector<int>{0,0,0,0,0,0,0,0,0,2},
    vector<int>{0,0,1,0,0,0,0,0,0,2},
    vector<int>{0,0,0,0,0,0,2,2,0,0}
  }
};




#endif