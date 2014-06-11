#ifndef POINTER_UNION_HPP
#define POINTER_UNION_HPP
/*******************************************************************************
 * \file
 * This is the header file containing pointer unions.
 *
 * \author Markus Saers <masaers@gmail.com>
 ******************************************************************************/
#include <type_traits>
#include <cstdint>
#include <cstddef>


/*******************************************************************************
 * A pointer union can store a pointer to fixed set of types, and
 * remembers what it currently stores; like a tagged union for
 * pointers.
 *
 * This is useful when you have storage of values or objects already
 * under controll, but whish to use them elsewhere in a homogenous way
 * (eg. by putting pointers to them in an STL container). There is no
 * need for a base class, and the values/objects themselves do not
 * necessarily share a meaningful API.
 *
 * \author Markus Saers <masaers@gmail.com>
 ******************************************************************************/
template<typename...> struct ptrunion;


/*******************************************************************************
 * The base case is ithe pointer union of no types; it can hold a
 * <code>void*</code>.
 *
 * \author Markus Saers <masaers@gmail.com>
 ******************************************************************************/
template<>
struct ptrunion<> {
public:
  typedef uint8_t size_type;
  enum : size_type { my_id = 0 };
  inline ptrunion() : ptr_m(NULL), id_m(my_id) {}
  template<typename T>
  inline ptrunion(T* ptr) : ptr_m(ptr), id_m(my_id) {}
  inline ptrunion(const ptrunion&) = default;
  inline ptrunion& operator=(const ptrunion& x) {
    ptr_m = x.ptr_m;
    id_m  = x.id_m;
    return *this;
  }
  inline ptrunion& operator=(void* ptr) {
    set(ptr, my_id);
    return *this;
  }
  inline bool operator==(const ptrunion& x) const {
    return ptr_m == x.ptr_m && id_m == x.id_m;
  }
  inline bool operator!=(const ptrunion& x) const { return ! operator==(x); }
  inline bool operator<(const ptrunion& x) const {
    return id_m != x.id_m ? id_m < x.id_m : ptr_m < x.ptr_m;
  }
  inline void* ptr() const { return ptr_m; }
  inline size_type id() const { return id_m; }
  inline operator void*() const { return ptr_m; }
protected:
  template<typename ptr_T>
  inline ptrunion(ptr_T* ptr, size_type id) : ptr_m(ptr), id_m(id) {}
  inline void set(void* ptr, size_type id) {
    ptr_m = ptr;
    id_m = id;
  }
private:
  void* ptr_m;
  size_type id_m;
};


/*******************************************************************************
 * The inductive case of the pointer union has at least one type
 * specified, allowing it to hold pointers to that type.
 *
 * \author Markus Saers <masaers@gmail.com>
 ******************************************************************************/
template<typename T, typename... Ts>
struct ptrunion<T, Ts...> : public ptrunion<Ts...> {
  typedef ptrunion<Ts...> base_type;
public:
  typedef typename base_type::size_type size_type;
  enum : size_type { my_id = base_type::my_id + 1 };
  inline ptrunion() : base_type() {}
  inline ptrunion(T* ptr) : base_type(ptr, my_id) {}
  inline ptrunion& operator=(T* ptr) {
    this->set((void*)ptr, my_id);
    return *this;
  }
  template<typename ptr_T> inline ptrunion& operator=(ptr_T* ptr) {
    base_type::operator=(ptr);
    return *this;
  }
  inline operator T*() const {
    T* result = NULL;
    if (this->id() == my_id) {
      result = static_cast<T*>(this->ptr());
    }
    return result;
  }
  template<typename ptr_T> inline operator ptr_T*() const {
    return base_type::operator ptr_T*();
  }
protected:
  template<typename ptr_T>
  inline ptrunion(ptr_T* ptr, size_type id) : base_type(ptr, id) {}
};


/******************************************************************************/
#endif
