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
    cout << *p.first << " " << (p.second ? "true" : "false") << endl;
  }
  {
    auto p = ps.insert(int(1));
    cout << *p.first << " " << (p.second ? "true" : "false") << endl;
  }
  {
    auto p = ps.insert(int(1));
    cout << *p.first << " " << (p.second ? "true" : "false") << endl;
  }
  {
    auto it = ps.find(int(1));
    cout << *it << endl;  
  }
  return EXIT_SUCCESS;
}