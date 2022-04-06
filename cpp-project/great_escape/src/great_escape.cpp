#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main() {
    try {
        
    } catch (const exception& e)  {
        Success(false);
        Message("Oops! ", e.what());
    }

  return 0;
}
