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

namespace com_masaers {

/*******************************************************************************
 * A pointer union can store a pointer to a fixed set of types, and
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
 * The base case is the pointer union of no types; it can hold a
 * <code>void*</code>.
 *
 * \author Markus Saers <masaers@gmail.com>
 ******************************************************************************/
template<>
struct ptrunion<> {
public:
  typedef uint8_t size_type;
protected:
  enum : size_type { my_id = 0 };
public:
  template<typename U, bool dummy = true> struct id_of {
    enum : size_type { value = my_id };
  };
  ///
  /// \name Constructors and destructors
  ///
  /// \{
  // ---------------------------------------------------------------------------
  ///
  /// Default constructor.
  ///
  inline ptrunion() : ptr_m(NULL), id_m(my_id) {}
  ///
  /// Constructor with any pointer type.
  ///
  template<typename ptr_T>
  inline ptrunion(/// The pointer to construct the pointer union with.
                  ptr_T* ptr) : ptr_m((void*)ptr), id_m(my_id) {}
  ///
  /// Constructor with any pointer type and a specific type id.
  ///
protected:
  template<typename ptr_T>
  inline ptrunion(/// The pointer to construct the poiter union with.
                  ptr_T* ptr,
		              /// The type id of the pointer.
                  size_type id) : ptr_m((void*)ptr), id_m(id) {}
public:
  ///
  /// Copy constructor.
  ///
  inline ptrunion(/// The pointer union to copy.
                  const ptrunion&) = default;
  // ---------------------------------------------------------------------------
  /// \}

  
  ///
  /// \name Overloaded operators
  ///
  /// \{
  // ---------------------------------------------------------------------------
  ///
  /// Assignment from an existing pointer union.
  ///
  inline ptrunion& operator=(/// The pointer union to assign from.
    const ptrunion& x) {
    set(x.ptr_m, x.id_m);
    return *this;
  }
  ///
  /// Assignment from a <code>void*</code>.
  ///
  inline ptrunion& operator=(/// The pointer that the pointer union
			                       /// will point to.
                             void* ptr) {
    set(ptr, my_id);
    return *this;
  }
  ///
  /// Equality operator.
  ///
  inline bool operator==(/// The pointer union to compare to.
                         const ptrunion& x) const {
    return ptr_m == x.ptr_m && id_m == x.id_m;
  }
  ///
  /// Inequality operator.
  ///
  inline bool operator!=(/// The pointer union to compare to.
                         const ptrunion& x) const { return ! operator==(x); }
  ///
  /// Ordering operator.
  ///
  inline bool operator<(/// The pointer union to compare to.
                        const ptrunion& x) const {
    return id_m != x.id_m ? id_m < x.id_m : ptr_m < x.ptr_m;
  }
  ///
  /// Cast to <code>void*</code> operator.
  ///
  inline operator void*() const { return ptr_m; }
  // ---------------------------------------------------------------------------
  /// \}


  ///
  /// \name Accessors
  ///
  /// \{
  // ---------------------------------------------------------------------------
  ///
  /// Retrieve the type id of the pointer being held.
  ///
  inline size_type id() const { return id_m; }
  ///
  /// Retrieve a <code>void*</code> version of the pointer being held.
  ///
  inline void* ptr() const { return ptr_m; }
  ///
  /// Determines whether the the held pointer is of a specific
  /// type. All pointer unions are potentially pointing to
  /// <code>void</code>.
  ///
  template<typename ptr_T>
  inline typename std::enable_if<std::is_same<void, ptr_T>::value, bool>::type
  is() const { return true; }
  ///
  /// Determines whether the the held pointer is of a specific
  /// type. All pointer unions are potentially pointing to
  /// <code>void</code>.
  ///
  template<typename ptr_T>
  inline typename std::enable_if<! std::is_same<void, ptr_T>::value, bool>::type
  is() const { return false; }
  // ---------------------------------------------------------------------------
  /// \}

protected:
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
protected:
  enum : size_type { my_id = base_type::my_id + 1 };
public:
  template<typename U, bool dummy = true> struct id_of
  : public base_type::template id_of<U>
  {};
  template<bool dummy> struct id_of<T, dummy> {
    enum : size_type { value = my_id };
  };


  ///
  /// \name Constructors and destructors
  ///
  /// \{
  // ---------------------------------------------------------------------------
  ///
  /// Default constructor.
  ///
  inline ptrunion() : base_type() {}
  ///
  /// Constructor with the specific pointer type handled by this class.
  ///
  inline ptrunion(/// The pointer to construct the pointer union with.
                  T* ptr) : base_type(ptr, my_id) {}
  ///
  /// Constructor with any pointer type not handled by this class.
  ///
  template<typename ptr_T>
  inline ptrunion(/// The pointer to construct the pointer union with.
                  ptr_T* ptr) : base_type(ptr) {}
  ///
  /// Constructor with any pointer with a given type id.
  ///
protected:
  template<typename ptr_T>
  inline ptrunion(/// The pointer to construct the pointer union with.
                  ptr_T* ptr,
		              /// The type id of the pointer.
                  size_type id) : base_type(ptr, id) {}
public:
  // ---------------------------------------------------------------------------
  /// \}


  ///
  /// \name Overloaded operators
  ///
  /// \{
  // ---------------------------------------------------------------------------
  ///
  /// Assignment operator from the specific pointer type handled by
  /// this class.
  ///
  inline ptrunion& operator=(/// The pointer to assign to the pointer union.
                             T* ptr) {
    this->set((void*)ptr, my_id);
    return *this;
  }
  ///
  /// Assignment operator from any pointer not handled by this class.
  ///
  template<typename ptr_T>
  inline ptrunion& operator=(// The pointer to assign to the pointer union.
                             ptr_T* ptr) {
    base_type::operator=(ptr);
    return *this;
  }
  ///
  /// Cast operator to the pointer type handled by this class.
  ///
  inline operator T*() const {
    T* result = NULL;
    if (this->id() == my_id) {
      result = static_cast<T*>(this->ptr());
    }
    return result;
  }
  ///
  /// Cast operator to any pointer type not handled by this class.
  ///
  template<typename ptr_T>
  inline operator ptr_T*() const {
    return base_type::operator ptr_T*();
  }
  // ---------------------------------------------------------------------------
  /// \}


  ///
  /// \name Accessors
  ///
  /// \{
  // ---------------------------------------------------------------------------
  ///
  /// Determines whether the held pointer is of a specific type.
  ///
  template<typename ptr_T>
  typename std::enable_if<std::is_same<T, ptr_T>::value, bool>::type
  is() const { return this->id() == my_id; }
  ///
  /// Determines whether the held pointer is of a specific type.
  ///
  template<typename ptr_T>
  typename std::enable_if<! std::is_same<T, ptr_T>::value, bool>::type
  is() const { return base_type::template is<ptr_T>(); }
  // ---------------------------------------------------------------------------
  /// \}
};

} // namespace com_masaers


/******************************************************************************/
#endif
