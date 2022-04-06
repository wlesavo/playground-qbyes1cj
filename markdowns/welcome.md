# Introduction

Looking for ways to speed up some parts of my code I often heard of a mystical "bitshifts" and "bitboards". Although we do have an exclent playground from [MSmits](https://www.codingame.com/playgrounds/38626/optimizing-breadth-first-search) and [emh](https://www.codingame.com/playgrounds/58038/fast-connected-components-for-6x12-bitboard), which by the way I higly recommend reading prior to this article since we wont get into the details about the common BFS itself, I found it to not be enough to implement trully fast BFS myself for a different task and for many things I would have to come to mind by myself. 

So in this playground I will try to demonstrate a simple way to think about BFS using bitboards on example of three different CG games: [Great Escape](https://www.codingame.com/multiplayer/bot-programming/great-escape), [Amazonial](https://www.codingame.com/multiplayer/bot-programming/amazonial) and [Line Racing](https://www.codingame.com/multiplayer/bot-programming/line-racing) going for somewhat different goals but showcasing the same idea. In Great Escape we will find the length of a shortest path, which is a main heuristic in evauation function for this game, in Amazonial and Line Racing we will calculate the voronoi diagramms, which is also the main heuristics for those games while also taking the most of a simulation time to calculate. The difference for a Line Racing is that the board cant be reasonably stored in a single integer value since it has 600 cells and we will come up with a possible workaround for this. 

I would also encourage more experienced users to point out to any mistakes I made or a bad practicies I had accidentally used.

# Storing connections for a grid

A common way to store a 2-dimensional array is to put in 1-dimensional array performing a simple transformation for coordinates
``` cpp
int pos = x + y * WIDTH;
```
with a back transformation 
``` cpp
int x = pos%WIDTH;
int y = pos/WIDTH;
```
To move pos say UP, we would have to do it the hard way
``` cpp
int x = pos%WIDTH;
int y = pos/WIDTH;
int new_x = x;
int new_y = y - 1;
// perform all necessary out of bounds checks
int new_pos = new_x + new_y * WIDTH;
```
But there is an unsafe method to do the same
``` cpp
int new_pos = pos - WIDTH;
```
This is easy, but we have to make sure that an impossible moves will be rejected.

Lets move on. To perform a simple BFS we will store connections for each direction in a single uint128_t value. Set bit for connection would mean that it is possible to walk from current possition to a corresponding direction. 
```
class State{
public:
    uint128_t con[4] = {};
    int player_pos = pos;    
}
```
To set or reset bit we will use a simple well known functions
``` cpp
template <class T>
static inline void set_bit(T& val, int n){
  val |= (T)1 << n;
}
template <class T>
static inline void reset_bit(T& val, int n){
  val &= ~((T)1 << n);
}
```
Preparing the state is described in the executed code example, although during the search we may simply turn on and off the connections needed, so that this connections will always be an an actualized state.

# Performing bit-shift BFS for finding the shortest path length

To perform a BFS we will need a containers to store current positions along with all visited positions. To move all current possitions in some direction (for example UP) we perform 'and' and 'shift' operations.
``` cpp
(cur&con[d])>>WIDTH;
```
Here 'and' opperation resets all currently stored positions to 0 if the move in chosen direction is impossible, making it possible to use the 'unsafe' move, described above. This way moving simultaneously in all directions is possible simply by merging all the moves with 'or'
for URDL. We also will keep track of all visits to not go back to visited cells.

``` cpp
int BFS(const State& s){
    uint128_t cur = {};
    set_bit(cur, s.player_pos);
    uint128_t visits = cur;
    int dist = 0;
    while (true) {
      if (cur==0){
        return dist;
      }
      cur = (cur&s.con[0])>>WIDTH | (cur&s.con[1])<<1 | (cur&s.con[2])<<WIDTH | (cur&s.con[3])>>1;
      cur &= ~visits;
      visits |= cur;
      dist += 1;
    }
    return -1;
}
```

The attentive reader would notice that here BFS misses win condition, so it performs a full scout until all reachable cells are visited. We will further fix this in the final executable example.

# Code example

@[Luke, how many stars are there in these galaxies?]({"stubs": ["src/Exercises/Universe.cpp"],"command": "sh /project/target/run.sh"})

Check out the markdown file [`welcome.md`](https://github.com/TechDotIO/cpp-template/blob/master/markdowns/welcome.md) to see how this exercise is injected into the template.


