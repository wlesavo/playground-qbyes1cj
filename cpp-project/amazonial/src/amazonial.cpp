//{
#pragma GCC optimize("O3,inline,omit-frame-pointer,unroll-loops","unsafe-math-optimizations","no-trapping-math")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,mmx,avx,avx2,popcnt,rdrnd,abm,bmi2,fma")
#ifdef __GNUC__
#include <cfenv>
#endif
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;
    
const int WIDTH = 8;
const int HEIGHT = 8;
const int SIZE = 8 * 8;

template <class T>
static int count_set_bits(T n)
{
  if (n == 0)
    return 0;
  else
    return (n & 1) + countSetBits(n >> 1);
}
template <class T>
static inline void set_bit(T& val, int n){
  val |= (T)1 << n;
}
template <class T>
static inline void reset_bit(T& val, int n){
  val &= ~((T)1 << n);
}
template <class T>
static inline bool check_bit(const T& a, int n){
  return (a >> n) & 1;
}

class State {
public:  
  uint64_t con[8] = {};
  int player_pos[2][2] = {};
  int voronoi_score[2] = {};
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
      cur_state.con[i] = ((uint64_t)1 << 64) - (uint64_t)1;
    }
    // remove boundary connections
    for (int i = 0; i < WIDTH; i++) {
      reset_bit(cur_state.con[0], i);
      reset_bit(cur_state.con[1], i);
      reset_bit(cur_state.con[1], i * WIDTH + WIDTH - 1);
      reset_bit(cur_state.con[2], i * WIDTH + WIDTH - 1);
      reset_bit(cur_state.con[3], i * WIDTH + WIDTH - 1);
      reset_bit(cur_state.con[3], i + WIDTH * (HEIGHT - 1));
      reset_bit(cur_state.con[4], i + WIDTH * (HEIGHT - 1));      
      reset_bit(cur_state.con[5], i + WIDTH * (HEIGHT - 1));
      reset_bit(cur_state.con[5], i * WIDTH);
      reset_bit(cur_state.con[6], i * WIDTH);
      reset_bit(cur_state.con[7], i * WIDTH);
      reset_bit(cur_state.con[7], i);
    }    
  }
  
  void voronoi(State& s){
  // set strating positions for both units
    for (int i = 0; i<2; i++){
      int pos1 = s.player_pos[0][i];
      set_bit(cur1, pos1);
      int pos2 = s.player_pos[1][i];
      set_bit(cur2, pos2);
    }
    uint64_t my_cells = cur1;
    uint64_t opp_cells = cur2;
    uint64_t visited = cur1 | cur2;
    voronoi_rolls += 1;
    
    while (cur1!=0 || cur2 != 0) {
      cur1 = (cur1 & s.con[0]) >> WIDTH | (cur1 & s.con[1]) >> (WIDTH - 1) 
           | (cur1 & s.con[2]) << 1     | (cur1 & s.con[3]) << (WIDTH + 1) 
           | (cur1 & s.con[4]) << WIDTH | (cur1 & s.con[5]) << (WIDTH - 1) 
           | (cur1 & s.con[6]) >> 1     | (cur1 & s.con[7]) >> (WIDTH + 1);
      cur1 &= ~visited;
    
      cur2 = (cur2 & s.con[0]) >> WIDTH | (cur2 & s.con[1]) >> (WIDTH - 1) 
           | (cur2 & s.con[2]) << 1     | (cur2 & s.con[3]) << (WIDTH + 1) 
           | (cur2 & s.con[4]) << WIDTH | (cur2 & s.con[5]) << (WIDTH - 1) 
           | (cur2 & s.con[6]) >> 1     | (cur2 & s.con[7]) >> (WIDTH + 1);
      cur2 &= ~visited;
      
      my_cells  |= (cur1 & (~cur2));
      opp_cells |= (cur2 & (~cur1));
    
      visited |= cur1 | cur2;
    }
    s.voronoi_score[0] = count_set_bits(my_cells)
    s.voronoi_score[1] = count_set_bits(opp_cells);
}

  void init_cur_state(){
    //init player position
    cur_state.player_pos[0][0] = 2 + 4 * WIDTH;
    cur_state.player_pos[0][1] = 3 + 5 * WIDTH;
    cur_state.player_pos[1][0] = 1 + 2 * WIDTH;
    cur_state.player_pos[1][1] = 4 + 6 * WIDTH;
    
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
    int roll_count = 1000000;
    auto start = high_resolution_clock::now();
    for (int i = 0; i<roll_count; i++){
        voronoi(cur_state);
    }
    auto end = high_resolution_clock::now();
    auto full_count = duration_cast<microseconds>(end - start).count()/1000;
    cerr << "total execution time " << full_count << "ms with " << voronoi_rolls << " rolls"<<  endl;
    cerr << "calculated voronoi score player 1: " << cur_state.voronoi_score[0] << " player 2: " << cur_state.voronoi_score[1] << endl;
  }
};

int main()
{
  Game g = Game();
  g.test();
}