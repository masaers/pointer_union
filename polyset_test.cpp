#include "polyset.hpp"
#include <set>
#include <iostream>
#include <cstdlib>

int main(const int argc, const char** argv) {
  using namespace std;
  using namespace com_masaers;

  polyset<double, int> ps;
  {
    auto p = ps.insert(double(1.5));
    cout << *(const double*)p.first << " " << (p.second ? "true" : "false") << endl;
  }
  {
    auto p = ps.insert(int(1));
    cout << *(const int*)p.first << " " << (p.second ? "true" : "false") << endl;
  }
  {
    auto p = ps.insert(int(1));
    cout << *(const int*)p.first << " " << (p.second ? "true" : "false") << endl;
  }
  {
    auto it = ps.find(int(1));
    cout << *(const int*)it << endl;  
  }
  {
    auto it = ps.find(int(1));
    cout << ((const int*)it != NULL ? "found!" : "missing!") << endl;
    ps.erase(int(1));
    auto jt = ps.find(int(1));
    cout << ((const int*)jt != NULL ? "found!" : "missing!") << endl;
  }
  return EXIT_SUCCESS;
}
