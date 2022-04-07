# Line Racing

Line Racing (aka Tron) is a game with quite simple rules and simulation, although it does heavily relies on a Voronoi diagrams calculation. For quite some time now I thought to myself of how to implement the algortihm similar to the previous part on such a large board. I ended up writing my own class storring 5 uint128_t values with a shift operations looking like this

``` cpp
struct Word5{
public:
  uint128_t val[5] = {};
};

Word5 shift_bits_right(int n){
  Word5 out{};      
  const int rev_n = INT_SIZE-n;
  out.val[4] = (val[4] >> n) | (val[3] << (rev_n));
  out.val[3] = (val[3] >> n) | (val[2] << (rev_n));
  out.val[2] = (val[2] >> n) | (val[1] << (rev_n));
  out.val[1] = (val[1] >> n) | (val[0] << (rev_n));
  out.val[0] = out.val[0] >> n;
  return out;
}
```

Although it did actually worked, giving the same result as the vanila queue implementation, the speed up was only up to 3-4 times. Comming to create this playground, I realised, that C++ bitset<> allows to perform all logical operations I need. Eventually bitset implementation had close to exactly the same speed as my own implementation, laking the pain of writing all of the support functions. So I decided to demonstrate only the bitset version, although I would really like to hear if there are any more effective alternatives. Appart from bitsets this version differs from Amazonial only in minor details, so without further ado here is an executable example.

# Code example

@[Luke, how many stars are there in these galaxies?]({"stubs": ["line_racing/src/line_racing.cpp"],"command": "sh /project/target/run_line_racing.sh"})



