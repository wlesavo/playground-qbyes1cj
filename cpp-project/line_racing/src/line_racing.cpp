//{
#pragma GCC optimize("O3,inline,omit-frame-pointer,unroll-loops","unsafe-math-optimizations","no-trapping-math")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,mmx,avx,avx2,popcnt,rdrnd,abm,bmi2,fma")
#ifdef __GNUC__
#include <cfenv>
#endif
#include <iostream>
#include <chrono>
#include <bitset>
using namespace std;
using namespace std::chrono;

const int WIDTH = 30;
const int HEIGHT = 20;
const int SIZE = WIDTH * HEIGHT;

class State {
public:  
  bitset<SIZE> con[4] = {};
  int player_pos[4] = {};
  int voronoi_score[4] = {};
  State() {};
};
//}

class Game {
public:

  State cur_state{};
  int voronoi_rolls = 0;
  Game() {
    // activate all connections
    for (int i = 0; i < 4; i++){
      cur_state.con[i] = ~(cur_state.con[i]);
    }
    // remove boundary connections
    for (int i = 0; i < WIDTH; i++) {
      cur_state.con[0].reset(i);
      cur_state.con[2].reset(i + WIDTH * (HEIGHT - 1));
    }
    for (int i = 0; i < HEIGHT; i++) {
      cur_state.con[1].reset(i * WIDTH + WIDTH - 1);
      cur_state.con[3].reset(i * WIDTH);
    }
  }
  
  void voronoi(State& s){
    // set strating positions for both units
    bitset<600> cur[4] = {};
    for (int p = 0; p < 4; p++) {
      cur[p].set(s.player_pos[p]);
    }
    voronoi_rolls += 1;
    bitset<600> visits[4] = {};
    bitset<600> total{};
    for (int p = 0; p < 4; p++) {
      visits[p] = cur[p];
      total |= cur[p];
    }
    
    int dist = 0;
    int score = 0;
    const int w = 30;
    bool skip[4] = {};
    //*
    while (true) {
      for (int i = 0; i<4; i++){
        if (skip[i])
          continue;
        cur[i] = (cur[i]&s.con[0])>>w
               | (cur[i]&s.con[1])<<1
               | (cur[i]&s.con[2])<<w
               | (cur[i]&s.con[3])>>1;
        cur[i] &= ~total;
      }
      dist += 1;
      bitset<600> opp_total[4]{};
      for (int i = 0; i<4; i++){
        if (skip[i])
          continue;
        for (int j = 0; j<4; j++){
          if (i!=j)
            opp_total[i] |= cur[j];
        }
      }
      for (int i = 0; i<4; i++){
        if (skip[i])
          continue;
        visits[i]|=cur[i]&(~opp_total[i]);
      }
      bool flag = true;
      for (int i = 0; i<4; i++){
        skip[i] = !(cur[i].any());
        flag = flag && skip[i];
      }
      if (flag){
        break;
      }
      for (int i = 0; i<4; i++){
        total |= cur[i];
      }
    }
    for (int i = 0; i<4; i++){
      s.voronoi_score[i] = visits[i].count();
    }
  }

  void init_cur_state(){
    //init player position
    cur_state.player_pos[0] = 1 + 5 * WIDTH;
    cur_state.player_pos[1] = 10 + 15 * WIDTH;
    cur_state.player_pos[2] = 4 + 14 * WIDTH;
    cur_state.player_pos[3] = 23 + 17 * WIDTH;
    
  }
  
  void test() {
    system("cat /proc/cpuinfo | grep \"model name\" | head -1 >&2");
    system("cat /proc/cpuinfo | grep \"cpu MHz\" | head -1 >&2");

    ios::sync_with_stdio(false);
#ifdef __GNUC__
    feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

    init_cur_state();
    int total_dist = 0;
    int roll_count = 5000;
    auto start = high_resolution_clock::now();
    for (int i = 0; i<roll_count; i++){
        voronoi(cur_state);
    }
    auto end = high_resolution_clock::now();
    auto full_count = duration_cast<microseconds>(end - start).count()/1000;
    cerr << "total execution time " << full_count << "ms with " << voronoi_rolls << " rolls" <<  endl;
    cerr << "calculated voronoi score player 1: " << cur_state.voronoi_score[0] 
         << " player 2: " << cur_state.voronoi_score[1]
         << " player 3: " << cur_state.voronoi_score[2]
         << " player 4: " << cur_state.voronoi_score[3] << endl;
  }
};

int main()
{
  Game g = Game();
  g.test();
}