# Great Escape

Lets proceed with an example, but first we need some brief introduction to general movement on grid operations.

# Coordinates and connections on a grid

A common way to store a 2-dimensional array is to put it in 1-dimensional array performing a simple transformation for coordinates
``` cpp
int pos = x + y * WIDTH;
```
with a back transformation 
``` cpp
int x = pos%WIDTH;
int y = pos/WIDTH;
```
To move position say UP, we would have to do it the hard way
``` cpp
int x = pos%WIDTH;
int y = pos/WIDTH;
int new_x = x;
int new_y = y - 1;
// perform all necessary out of bounds checks
int new_pos = new_x + new_y * WIDTH;
```
But there is also an equivalent unsafe method to do the same
``` cpp
int new_pos = pos - WIDTH;
```
This is quite faster, but we have to make sure that an impossible moves will be rejected.

Lets move on. To perform a simple BFS we will store connections for each direction in a single uint128_t value. Set bit for connection would mean that it is possible to walk from current possition to a corresponding direction. 
``` cpp
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
Preparing the state is described in the executed code example, although during the search we may simply turn on and off the connections needed, so that this connections will always be in an actualized state.

# Performing bit-shift BFS for finding the shortest path length

To perform a BFS we will need a containers to store current positions along with all visited positions. To move all current possitions in some direction (for example UP) we perform 'and' and 'shift' operations.
``` cpp
(cur&con[d])>>WIDTH;
```
Here 'and' opperation resets all currently stored positions to 0 if the move in chosen direction is impossible, making it ready to use the 'unsafe' move, described above. This way moving simultaneously in all directions is performed by simply merging all the moves for URDL with 'or'. We also will have to keep track of all visits to not go back to visited cells. Basically we are done, a simple BFS then will look like this

``` cpp
int BFS(const State& s){
    uint128_t cur = 0;
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

In the executable example the connections preparation and win masks calculation are demonstrated. 

@[Great escape shortest path calculation example]({"stubs": ["/great_escape/src/great_escape.cpp"], "command": "sh /project/target/run_ge.sh"})



