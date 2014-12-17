#include "pointer_union.hpp"
#include <cassert>


int main(const int argc, const char** argv) {
  using namespace std;
  using namespace com_masaers;
  
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
    assert(  pu.is<int>());
    assert(! pu.is<const int>());
    assert(! pu.is<double>());
    assert(  pu.is<void>());
    pu = &ci;
    assert((const int*)pu == &ci);
    assert(     (void*)pu == &ci);
    assert(      (int*)pu == NULL);
    assert((const int*)pu != NULL);
    assert(   (double*)pu == NULL);
    assert(! pu.is<int>());
    assert(  pu.is<const int>());
    assert(! pu.is<double>());
    assert(  pu.is<void>());
    pu = &d;
    assert(   (double*)pu == &d);
    assert(     (void*)pu == &d);
    assert(      (int*)pu == NULL);
    assert((const int*)pu == NULL);
    assert(   (double*)pu != NULL);
    assert(! pu.is<int>());
    assert(! pu.is<const int>());
    assert(  pu.is<double>());
    assert(  pu.is<void>());
  }
  {
    ptrunion<int, double> pu(&d);
    pu = &ci;
    assert(pu.is<void>());
    assert((void*)pu == &ci);
  }

  {
    typedef ptrunion<int, const int, double> pu_type;
    pu_type pu(&i);
    switch (pu.id()) {
    case pu_type::id_of<int>::value : {
      break; }
    case pu_type::id_of<const int>::value : {
      assert(false);
      break; }
    case pu_type::id_of<double>::value : {
      assert(false);
      break; }
    case pu_type::id_of<void>::value : {
      assert(false);
      break; }
    };
  }
  {
    typedef ptrunion<int, const int, double> pu_type;
    pu_type pu(&ci);
    switch (pu.id()) {
    case pu_type::id_of<const int>::value : {
      break; }
    default : {
      assert(false);
      break; }
    };
  }
  {
    typedef ptrunion<int, const int, double> pu_type;
    pu_type pu(&d);
    switch (pu.id()) {
    case pu_type::id_of<double>::value : {
      break; }
    default : {
      assert(false);
      break; }
    };
  }
  {
    typedef ptrunion<int, const int, double> pu_type;
    pu_type pu;
    switch (pu.id()) {
    case pu_type::id_of<void>::value : {
      break; }
    default : {
      assert(false);
      break; }
    };
  }
  
  return EXIT_SUCCESS;
}

