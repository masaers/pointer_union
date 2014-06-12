#include "pointer_union.hpp"
#include <cassert>


int main(const int argc, const char** argv) {
  using namespace std;
  
  int i = 2;
  const int ci = 5;
  double d = 3.14;
  
  {
    ptrunion<int, const int, double> pu(&i);
    assert(      (int*)pu == &i);
    assert(     (void*)pu == &i);
    assert(      (int*)pu != NULL);
    assert((const int*)pu == NULL);
    assert(   (double*)pu == NULL);
    pu = &ci;
    assert((const int*)pu == &ci);
    assert(     (void*)pu == &ci);
    assert(      (int*)pu == NULL);
    assert((const int*)pu != NULL);
    assert(   (double*)pu == NULL);
    pu = &d;
    assert(   (double*)pu == &d);
    assert(     (void*)pu == &d);
    assert(      (int*)pu == NULL);
    assert((const int*)pu == NULL);
    assert(   (double*)pu != NULL);
  }
  {
    ptrunion<int, double> pu(&d);
  }
  
  return EXIT_SUCCESS;
}

