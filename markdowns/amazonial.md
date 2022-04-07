# Amazonial
As was previously said, we will now try to apply similar concept to solve a quite different task. Namely we will calculate the number of cells one player can reach faster than another and vise versa, which is a special case of [Voronoi diagrams](https://www.codingame.com/playgrounds/243/voronoi-diagrams/fortunes-algorithm) calculated on a grid.

# General idea
The game Amazonial is not that different from the Great Escape if you look at it at the right angle. Although it still requires quite a few modifications, i.e accounting for multiple units and 8 available dirrections. The later will lead to a modification of our BFS as

``` cpp
cur = (cur & con[0]) >> WIDTH | (cur & con[1]) >> (WIDTH - 1) 
    | (cur & con[2]) << 1     | (cur & con[3]) << (WIDTH + 1) 
    | (cur & con[4]) << WIDTH | (cur & con[5]) << (WIDTH - 1) 
    | (cur & con[6]) >> 1     | (cur & con[7]) >> (WIDTH + 1);
```

The rest is simple. We first calculate new positions on a current depth for both players, by expanding current cells and substracting all the cells already visited by any player. Then the cells that only one player reached on a current depth are added to the positions that has been won. Then we finally update visited cells. After the BFSs for both players are finished, we calculate the area by counting set bits with a simple recursive function.

``` cpp
template <class T>
static int count_set_bits(T n)
{
  if (n == 0)
    return 0;
  else
    return (n & 1) + countSetBits(n >> 1);
}

void voronoi(const State& s){
  // set strating positions for both units
  for (int i = 0; i<2; i++){
    int pos1 = s.players[0].pos[i];
    set_bit(cur1, pos1);
    int pos2 = s.players[1].pos[i];
    set_bit(cur2, pos2);
  }
  uint64_t my_cells = cur1;
  uint64_t opp_cells = cur2;
  uint64_t visited = cur1 | cur2;
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
  int my_score = count_set_bits(my_cells)
  int opp_score = count_set_bits(opp_cells);
}
```

# Code example

@[Amazonial voronoi calculation]({"stubs": ["amazonial/src/amazonial.cpp"],"command": "sh /project/target/run_amazonial.sh"})

